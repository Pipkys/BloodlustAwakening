#include "BloodlustAwakening/Actors/Buildings/CppBuilding.h"

#include "BloodlustAwakening/Core/Physics/CppCollisionChannels.h"
#include "BloodlustAwakening/GameMode/CppBloodlustAwakeningGameplayStatics.h"
#include "BloodlustAwakening/Subsystems/CppBuildingRegistry.h"

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
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Unit, ECollisionResponse::ECR_Overlap);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Building, ECollisionResponse::ECR_Overlap);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Projectile, ECollisionResponse::ECR_Overlap);
	CapsuleComponent->SetCanEverAffectNavigation(true);
	CapsuleComponent->SetCapsuleRadius(34.0f);
	CapsuleComponent->SetCapsuleHalfHeight(88.0f);

	NavModifierComponent = CreateDefaultSubobject<UNavModifierComponent>("NavModifierComponent");
}

void ACppBuilding::BeginPlay()
{
	Super::BeginPlay();

	TeamSubsystem = GetWorld()->GetSubsystem<UCppTeamSubsystem>();
	StatsSubsystem = GetWorld()->GetSubsystem<UCppStatsSubsystem>();
}

void ACppBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACppBuilding::Setup(const FCppBuildingSpawnParameters& InSpawnParameters)
{
	TeamComponent->Team = InSpawnParameters.Team;
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

FCppStats* ACppBuilding::GetStats(ECppStatsType InType)
{
	return StatsComponent->GetStats(GetClass(), InType);
}

void ACppBuilding::CacheStats(const FCppStatsView& InView)
{
	StatsComponent->LoadedStats.Add(InView);
}

bool ACppBuilding::IsHostileTo(TScriptInterface<ICppEntity> InOther) const
{
	return TeamSubsystem->IsHostile((UObject*)this, InOther);
}

float ACppBuilding::TakeDamage(float InBaseDamage,
	struct FDamageEvent const& InDamageEvent,
	class AController* InEventInstigator,
	AActor* InDamageCauser)
{
	return UCppBloodlustAwakeningGameplayStatics::IsInvincible(this)
		? 0.f
		: Super::TakeDamage(InBaseDamage, InDamageEvent, InEventInstigator, InDamageCauser);
}

void ACppBuilding::OnDie()
{
	GetWorld()->GetSubsystem<UCppBuildingRegistry>()->UnregisterBuilding(this);
}

int32 ACppBuilding::GetSizeInCells() const
{
	return SizeInCells;
}

TArray<ACppCell*>& ACppBuilding::GetOccupiedCells()
{
	return OccupiedCells;
}
