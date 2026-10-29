#include "BloodlustAwakening/Components/AttackComponent/CppAttackComponent.h"

#include "BloodlustAwakening/Components/AttackComponent/CppAttackComponentStats.h"
#include "BloodlustAwakening/GameMode/CppBloodlustAwakeningGameplayStatics.h"
#include "BloodlustAwakening/Subsystems/CppStatsSubsystem.h"

void UCppAttackComponent::SetupComponent(AActor* InOwner, const FCppActorSpawnParameters& InSpawnParameters)
{
	if (FCppStats* Stats = GetWorld()->GetSubsystem<UCppStatsSubsystem>()->LoadStats(InOwner, AttackType == ECppAttackType::Melee 
			? ECppStatsType::MeleeAttack
			: ECppStatsType::RangedAttack))
	{
		FCppAttackComponentStats* AttackStats = (FCppAttackComponentStats*)Stats;
		DamageType = AttackStats->DamageType;
		AttackEffect = AttackStats->AttackEffect;
		Damage = AttackStats->Damage;
		Speed = AttackStats->Speed;
		Range = AttackStats->Range;
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("UCppAttackComponent::SetupComponent: Failed to load attack stats"));
}

bool UCppAttackComponent::IsTargetInAttackRange(AActor* InTarget, const FTransform& InPointTransform) const
{
 	return FVector::Distance(InPointTransform.GetLocation(), InTarget->GetActorLocation()) <= Range;
}

bool UCppAttackComponent::IsCanAttack() const
{
	if (!AttackTimer.IsValid())
	{
		return true;
	}

	return false;
}

bool UCppAttackComponent::IsCanAttackTarget(AActor* InTarget, const FTransform& InPointTransform) const
{
	if (IsCanAttack() 
		&& IsTargetInAttackRange(InTarget, InPointTransform) 
		&& !UCppBloodlustAwakeningGameplayStatics::IsInvincible(InTarget))
	{
		return true;
	}

	return false;
}

void UCppAttackComponent::SetAttackTimerCallaback(UObject* InObject, const FName& InFunctionName)
{
	FScriptDelegate Delegate;
	Delegate.BindUFunction(InObject, InFunctionName);
	AttackTimerCallback.Add(Delegate);
}

void UCppAttackComponent::OnPerformAttack() const
{
	auto Callback = [this]()
	{
		if (AttackTimerCallback.IsBound())
		{
			AttackTimerCallback.Broadcast();
		}
		AttackTimer.Invalidate();
	};

	GetWorld()->GetTimerManager().SetTimer(
		AttackTimer,
		Callback,
		Speed,
		false);
}
