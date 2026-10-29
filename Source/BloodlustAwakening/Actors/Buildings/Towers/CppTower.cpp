#include "BloodlustAwakening/Actors/Buildings/Towers/CppTower.h"

#include "BloodlustAwakening/Actors/Buildings/Towers/CppTowerSpawnParameters.h"
#include "BloodlustAwakening/Actors/Buildings/Towers/CppTowerStats.h"
#include "BloodlustAwakening/Components/AttackComponent/Ranged/CppRangedAttackComponent.h"
#include "BloodlustAwakening/Core/Physics/CppCollisionChannels.h"
#include "BloodlustAwakening/Subsystems/CppStatsSubsystem.h"
#include "BloodlustAwakening/Subsystems/CppTeamSubsystem.h"

ACppTower::ACppTower()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UCppHealthComponent>("HealthComponent");
	AttackComponent = CreateDefaultSubobject<UCppRangedAttackComponent>("RangedAttackComponent");

	AttackPointComponent = CreateDefaultSubobject<USphereComponent>("AttackPointComponent");
	AttackPointComponent->SetupAttachment(RootComponent);
	AttackPointComponent->SetCollisionObjectType(ECC_Trigger);
	AttackPointComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackPointComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackPointComponent->SetCanEverAffectNavigation(false);
}

void ACppTower::BeginPlay()
{
	Super::BeginPlay();
}

void ACppTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACppTower::Setup(const FCppBuildingSpawnParameters& InSpawnParameters)
{
	AttackPointComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackPointComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackPointComponent->SetCanEverAffectNavigation(false);
	
	FCppTowerStats* TowerStats = (FCppTowerStats*)StatsSubsystem->LoadStats(this, ECppStatsType::Tower);
	if (!TowerStats)
	{
		UE_LOG(LogTemp, Error, TEXT("ACppTower::Setup: Failed to load tower stats"));
	}

	HealthComponent->SetupComponent(this, InSpawnParameters);
	AttackComponent->SetupComponent(this, InSpawnParameters);
}

float ACppTower::TakeDamage(float InBaseDamage,
	struct FDamageEvent const& InDamageEvent,
	class AController* InEventInstigator,
	AActor* InDamageCauser)
{
	float BaseDamage = Super::TakeDamage(InBaseDamage, InDamageEvent, InEventInstigator, InDamageCauser);
	HealthComponent->OnTakeDamage(BaseDamage, InDamageEvent, InEventInstigator, InDamageCauser);
	float Diff = HealthComponent->CurrentHealth - BaseDamage;
	HealthComponent->SetCurrentHealth(FMath::Clamp(Diff, 0.f, HealthComponent->MaxHealth));
	if (Diff <= 0.f)
	{
		Die();
	}
	return BaseDamage;
}

UCppHealthComponent* ACppTower::GetHealthComponent()
{
	return HealthComponent;
}

float ACppTower::GetMaxHealth() const
{
	return HealthComponent->GetMaxHealth();
}

void ACppTower::SetMaxHealth(float InMaxHealth)
{
	HealthComponent->SetMaxHealth(InMaxHealth);
}

float ACppTower::GetCurrentHealth() const
{
	return HealthComponent->GetMaxHealth();
}

void ACppTower::SetCurrentHealth(float InCurrentHealth)
{
	HealthComponent->SetCurrentHealth(InCurrentHealth);
}

bool ACppTower::IsAlive() const
{
	return HealthComponent->IsAlive();
}

bool ACppTower::IsDead() const
{
	return HealthComponent->IsDead();
}

void ACppTower::SubscribeToOnTakeDamageEvent(UObject* InObject, const FName& InFunctionName)
{
	HealthComponent->SubscribeToOnTakeDamageEvent(InObject, InFunctionName);
}

void ACppTower::SubscribeToOnDieEvent(UObject* InObject, const FName& InFunctionName)
{
	HealthComponent->SubscribeToOnDieEvent(InObject, InFunctionName);
}

void ACppTower::Die()
{
	OnDie();
	HealthComponent->OnDie();
	Destroy();
}

UCppAttackComponent* ACppTower::GetAttackComponent()
{
	return AttackComponent;
}

USphereComponent* ACppTower::GetAttackPointComponent()
{
	return AttackPointComponent;
}

AActor* ACppTower::GetCurrentTarget() const
{
	return CurrentTarget;
}

void ACppTower::LockOnTarget(AActor* InTarget)
{
	bLockedOnTarget = true;
	CurrentTarget = InTarget;
}

void ACppTower::ReleaseTarget()
{
	bLockedOnTarget = false;
	CurrentTarget = nullptr;
}

bool ACppTower::IsTargetInAttackRange(AActor* InTarget, const FTransform& InAttackPointTransform) const
{
	return AttackComponent->IsTargetInAttackRange(InTarget, InAttackPointTransform);
}

bool ACppTower::IsCanAttack() const
{
	return AttackComponent->IsCanAttack();
}

bool ACppTower::IsCanAttackTarget(AActor* InTarget, const FTransform& InAttackPointTransform) const
{
	return AttackComponent->IsCanAttackTarget(InTarget, InAttackPointTransform);
}
