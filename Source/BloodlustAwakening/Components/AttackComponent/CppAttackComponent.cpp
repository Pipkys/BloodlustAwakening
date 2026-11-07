#include "BloodlustAwakening/Components/AttackComponent/CppAttackComponent.h"

#include "BloodlustAwakening/Components/AttackComponent/CppAttackStats.h"
#include "BloodlustAwakening/Components/AttackComponent/CppAttackParams.h"
#include "BloodlustAwakening/Core/Stats/CppStats.h"
#include "BloodlustAwakening/Interface/CppEntity.h"
#include "BloodlustAwakening/Subsystems/CppStatsSubsystem.h"

#include "Engine/World.h"
#include "TimerManager.h"

UCppAttackComponent::UCppAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCppAttackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(AttackTimer);
	}

	Super::EndPlay(EndPlayReason);
}

void UCppAttackComponent::SetupComponent(AActor* InOwner, const FCppActorSpawnParameters& InSpawnParameters)
{
	FCppAttackStats* Stats = UCppStatsSubsystem::LoadStats<ECppStatsType::Attack>(InOwner);
	if (!Stats)
	{
		UE_LOG(LogTemp, Error, TEXT("UCppAttackComponent::SetupComponent: failed to load attack stats for '%s'"), *GetNameSafe(InOwner));
		return;
	}

	AttackParams = Stats->AttackParams.GetPtr<FCppAttackParams>();
	AttackBehavior = NewObject<UCppAttackBehavior>(InOwner, AttackParams->AttackBehavior, "AttackBehavior");
	AttackBehavior->SetupBehavior(InOwner, Stats->AttackParams);
}

bool UCppAttackComponent::IsTargetInAttackRange(AActor* InTarget, const FTransform& InPointTransform) const
{
	const float AttackRange = AttackParams->Range;
	if (AttackRange <= 0.f)
	{
		return false;
	}

	FVector TargetOrigin;
	FVector TargetExtent;
	InTarget->GetActorBounds(true, TargetOrigin, TargetExtent);

	const double TargetRadius = FMath::Max(TargetExtent.X, TargetExtent.Y);
	const double EffectiveRange = AttackRange + TargetRadius;
	const double EffectiveRangeSquared = EffectiveRange * EffectiveRange;

	const double CenterDistanceSquared = FVector::DistSquared2D(InPointTransform.GetLocation(), InTarget->GetActorLocation());
	return CenterDistanceSquared <= EffectiveRangeSquared;
}

bool UCppAttackComponent::IsCanAttack() const
{
	const UWorld* World = GetWorld();
	return World ? !World->GetTimerManager().IsTimerActive(AttackTimer) : false;
}

bool UCppAttackComponent::IsCanAttackTarget(AActor* InTarget, const FTransform& InPointTransform) const
{
	return IsCanAttack() && IsTargetInAttackRange(InTarget, InPointTransform);
}

const FCppAttackParams& UCppAttackComponent::GetAttackParams() const
{
	return *AttackParams;
}

void UCppAttackComponent::PerformAttack(AActor* InTarget, const FTransform& InAttackPointTransform)
{
	AttackBehavior->PerformAttack(InTarget, InAttackPointTransform);
	StartAttackCooldown();
}

void UCppAttackComponent::SetAttackTimerCallback(UObject* InObject, const FName& InFunctionName)
{
	FScriptDelegate Delegate;
	Delegate.BindUFunction(InObject, InFunctionName);
	AttackTimerCallback.AddUnique(Delegate);
}

void UCppAttackComponent::StartAttackCooldown()
{
	GetWorld()->GetTimerManager().SetTimer(AttackTimer,
		FTimerDelegate::CreateUObject(this, &UCppAttackComponent::OnAttackCooldownFinished),
		AttackParams->Speed,
		false);
}

void UCppAttackComponent::OnAttackCooldownFinished()
{
	AttackTimer.Invalidate();
	if (AttackTimerCallback.IsBound())
	{
		AttackTimerCallback.Broadcast();
	}
}
