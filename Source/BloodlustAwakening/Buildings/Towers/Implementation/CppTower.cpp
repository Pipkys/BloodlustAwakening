#include "BloodlustAwakening/Buildings/Towers/Implementation/CppTower.h"

#include "BloodlustAwakening/Buildings/Towers/Implementation/CppTowerSpawnParameters.h"
#include "BloodlustAwakening/Buildings/Towers/Implementation/CppTowerStats.h"
#include "BloodlustAwakening/Components/AttackComponent/CppAttackComponent.h"
#include "BloodlustAwakening/Core/Physics/CppCollisionChannels.h"
#include "BloodlustAwakening/Core/Stats/CppStats.h"
#include "BloodlustAwakening/GameMode/CppBloodlustAwakeningGameplayStatics.h"
#include "BloodlustAwakening/Subsystems/CppStatsSubsystem.h"

ACppTower::ACppTower()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UCppHealthComponent>("HealthComponent");
	AttackComponent = CreateDefaultSubobject<UCppAttackComponent>("AttackComponent");

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
	if (IsDead())
	{
		Die();
		return;
	}
	const FTransform& AttackPointTransform = AttackPointComponent->GetComponentTransform();
	if (IsValid(CurrentTarget))
	{
		if (IsCanAttackTarget(CurrentTarget, AttackPointTransform))
		{
			AttackComponent->PerformAttack(CurrentTarget, AttackPointTransform);
		}
		else
		{
			ReleaseTarget();
		}
	}
	else
	{
		if (bLockedOnTarget)
		{
			ReleaseTarget();
		}
		const float AttackRange = AttackComponent->GetRange();
		const TArray<AActor*>& DamageableActorsAround = UCppBloodlustAwakeningGameplayStatics::FindActors(this,
			AttackRange,
			{ ECC_Unit },
			ICppDamageable::UClassType::StaticClass(),
			{ this },
			AttackPointTransform.GetLocation());
		if (DamageableActorsAround.IsEmpty())
		{
			return;
		}
		AActor* NearestDamageableActor = UCppBloodlustAwakeningGameplayStatics::FindNearestActor(DamageableActorsAround,
			AttackRange,
			AttackPointTransform.GetLocation());
		if (IsValid(NearestDamageableActor) && IsHostileTo(NearestDamageableActor))
		{
			LockOnTarget(NearestDamageableActor);
		}
	}
}

void ACppTower::Setup(const FCppBuildingSpawnParameters& InSpawnParameters)
{
	Super::Setup(InSpawnParameters);

	AttackPointComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackPointComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackPointComponent->SetCanEverAffectNavigation(false);

	if (!UCppStatsSubsystem::LoadCppStatsAs<ECppStatsType::Tower>(this))
	{
		UE_LOG(LogTemp, Error, TEXT("ACppTower::Setup: Failed to load Tower stats"));
	}

	HealthComponent->SetupComponent(this, InSpawnParameters);
	AttackComponent->SetupComponent(this, InSpawnParameters);
}

float ACppTower::TakeDamage(float InBaseDamage,
	struct FDamageEvent const& InDamageEvent,
	class AController* InEventInstigator,
	AActor* InDamageCauser)
{
	if (UCppBloodlustAwakeningGameplayStatics::IsInvincible(this))
	{
		return 0.0f;
	}
	return HealthComponent
		? HealthComponent->OnTakeDamage(InBaseDamage, InDamageEvent, InEventInstigator, InDamageCauser)
		: 0.0f;
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
	return HealthComponent->GetCurrentHealth();
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
	// Unregister from the building registry, then destroy. The death event
	// itself is broadcast by the health component before we get here.
	OnDie();
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
