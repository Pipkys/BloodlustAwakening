#include "BloodlustAwakening/Buildings/Towers/Implementation/CppTower.h"

#include "BloodlustAwakening/Buildings/Towers/Implementation/CppTowerStats.h"
#include "BloodlustAwakening/Core/Physics/CppCollisionChannels.h"
#include "BloodlustAwakening/Core/Stats/CppStats.h"
#include "BloodlustAwakening/GameMode/CppGameplayStatics.h"
#include "BloodlustAwakening/Subsystems/CppStatsSubsystem.h"

ACppTower::ACppTower()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.05f;

	HealthComponent = CreateDefaultSubobject<UCppHealthComponent>("HealthComponent");
	AttackComponent = CreateDefaultSubobject<UCppAttackComponent>("AttackComponent");

	AttackPointComponent = CreateDefaultSubobject<USphereComponent>("AttackPointComponent");
	AttackPointComponent->SetupAttachment(RootComponent);
	AttackPointComponent->SetCollisionObjectType(ECC_Trigger);
	AttackPointComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackPointComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackPointComponent->SetCanEverAffectNavigation(false);
}

void ACppTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead())
	{
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

		const float AttackRange = AttackComponent->GetAttackParams().Range;
		AActor* NearestDamageableHostile = UCppGameplayStatics::FindNearestActorByPredicate(UCppGameplayStatics::FindActors(this,
			AttackRange,
			{ ECC_Unit },
			AActor::StaticClass(),
			{ this },
			AttackPointTransform.GetLocation()),
			AttackPointTransform.GetLocation(),
			[this](AActor* InActor)
		{
			return IsHostileTo(InActor) && !UCppGameplayStatics::IsInvincible(InActor);
		});

		if (IsValid(NearestDamageableHostile))
		{
			LockOnTarget(NearestDamageableHostile);
		}
	}
}

void ACppTower::Setup(const FCppBuildingSpawnParameters& InSpawnParameters)
{
	Super::Setup(InSpawnParameters);

	HealthComponent->SetupComponent(this, InSpawnParameters);
	AttackComponent->SetupComponent(this, InSpawnParameters);
}

void ACppTower::LoadStats()
{
	if (!UCppStatsSubsystem::LoadStats<ECppStatsType::Tower>(this))
	{
		UE_LOG(LogTemp, Error, TEXT("ACppTower::LoadStats: failed to load tower stats"));
	}

	Super::LoadStats();
}

float ACppTower::TakeDamage(float InBaseDamage,
	struct FDamageEvent const& InDamageEvent,
	class AController* InEventInstigator,
	AActor* InDamageCauser)
{
	const float ActualDamage = Super::TakeDamage(InBaseDamage, InDamageEvent, InEventInstigator, InDamageCauser);
	return HealthComponent
		? HealthComponent->OnTakeDamage(ActualDamage, InDamageEvent, InEventInstigator, InDamageCauser)
		: ActualDamage;
}

UCppHealthComponent* ACppTower::GetHealthComponent()
{
	return HealthComponent;
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
