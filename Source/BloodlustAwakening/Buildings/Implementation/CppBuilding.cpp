#include "BloodlustAwakening/Buildings/Implementation/CppBuilding.h"

#include "BloodlustAwakening/Buildings/Implementation/CppBuildingStats.h"
#include "BloodlustAwakening/Components/WorldUIComponent/CppWorldUIComponent.h"
#include "BloodlustAwakening/Core/Physics/CppCollisionChannels.h"
#include "BloodlustAwakening/Core/Stats/CppStats.h"
#include "BloodlustAwakening/GameMode/CppGameplayStatics.h"
#include "BloodlustAwakening/Subsystems/CppStatsSubsystem.h"

ACppBuilding::ACppBuilding()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionObjectType(ECC_Building);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	StaticMeshComponent->SetCanEverAffectNavigation(false);

	TeamComponent = CreateDefaultSubobject<UCppTeamComponent>("TeamComponent");
	StatsComponent = CreateDefaultSubobject<UCppStatsComponent>("StatsComponent");

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->SetupAttachment(RootComponent);
	CapsuleComponent->SetCollisionObjectType(ECC_Building);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Overlap);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Unit, ECR_Overlap);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Building, ECR_Overlap);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	CapsuleComponent->SetCanEverAffectNavigation(true);
	CapsuleComponent->SetCapsuleRadius(34.0f);
	CapsuleComponent->SetCapsuleHalfHeight(88.0f);

	NavModifierComponent = CreateDefaultSubobject<UNavModifierComponent>("NavModifierComponent");

	WorldUIComponent = CreateDefaultSubobject<UCppWorldUIComponent>("WorldUIComponent");
	WorldUIComponent->SetupAttachment(RootComponent);
	WorldUIComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WorldUIComponent->SetDrawSize({ 50.f, 50.f });
}

void ACppBuilding::BeginPlay()
{
	Super::BeginPlay();
	TeamSubsystem = GetWorld()->GetSubsystem<UCppTeamSubsystem>();
}

void ACppBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACppBuilding::Setup(const FCppBuildingSpawnParameters& InSpawnParameters)
{
	TeamComponent->Team = InSpawnParameters.Team;
	LoadStats();
}

UStaticMeshComponent* ACppBuilding::GetStaticMeshComponent()
{
	return StaticMeshComponent;
}

UCppTeamComponent* ACppBuilding::GetTeamComponent()
{
	return TeamComponent;
}

UCppStatsComponent* ACppBuilding::GetStatsComponent()
{
	return StatsComponent;
}

UCppTeam* ACppBuilding::GetTeam() const
{
	return TeamComponent->Team;
}

bool ACppBuilding::IsHostileTo(TScriptInterface<ICppEntity> InOther) const
{
	return TeamSubsystem->IsHostile((UObject*)this, InOther);
}

void ACppBuilding::LoadStats()
{
	if (FCppBuildingStats* Stats = UCppStatsSubsystem::LoadStats<ECppStatsType::Building>(this))
	{
		SizeInCells = Stats->SizeInCells;
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("ACppBuilding::LoadStats: failed to load building stats"));
}

float ACppBuilding::TakeDamage(float InBaseDamage,
	struct FDamageEvent const& InDamageEvent,
	class AController* InEventInstigator,
	AActor* InDamageCauser)
{
	return UCppGameplayStatics::IsInvincible(this)
		? 0.f
		: Super::TakeDamage(InBaseDamage, InDamageEvent, InEventInstigator, InDamageCauser);
}

void ACppBuilding::ForceDie()
{
	if (ICppDamageable* DamageableInterface = Cast<ICppDamageable>(this))
	{
		DamageableInterface->Die();
		return;
	}

	Destroy();
}

int32 ACppBuilding::GetSizeInCells() const
{
	return SizeInCells;
}

TArray<ACppCell*>& ACppBuilding::GetOccupiedCells()
{
	return OccupiedCells;
}
