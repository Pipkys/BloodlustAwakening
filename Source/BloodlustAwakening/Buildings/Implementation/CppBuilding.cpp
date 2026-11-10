#include "BloodlustAwakening/Buildings/Implementation/CppBuilding.h"

#include "BloodlustAwakening/Buildings/Implementation/CppBuildingStats.h"
#include "BloodlustAwakening/Components/WorldUIComponent/CppWorldUIComponent.h"
#include "BloodlustAwakening/Core/Physics/CppCollisionChannels.h"
#include "BloodlustAwakening/Core/Stats/CppStats.h"
#include "BloodlustAwakening/GameMode/CppGameplayStatics.h"
#include "BloodlustAwakening/Subsystems/CppStatsSubsystem.h"

ACppBuilding::ACppBuilding()
{
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
