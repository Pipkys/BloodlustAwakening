#include "BloodlustAwakening/Components/AttackComponent/Melee/CppMeleeAttackBehavior.h"

#include "BloodlustAwakening/Components/AttackComponent/Melee/CppMeleeAttackParams.h"
#include "BloodlustAwakening/Core/Physics/CppCollisionChannels.h"
#include "BloodlustAwakening/GameMode/CppGameplayStatics.h"
#include "BloodlustAwakening/Interface/CppEntity.h"

#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

void UCppMeleeAttackBehavior::PerformAttack(AActor* InTarget, const FTransform& InAttackPoint)
{
	FCppMeleeAttackParams* AttackParams = GetParamsAs<FCppMeleeAttackParams>();
	const float EffectiveDamage = AttackParams->Damage;

	SpawnNiagaraEffect(AttackParams->AttackEffect, InAttackPoint);
	if (AttackParams->bSingleTarget)
	{
		if (IsValid(InTarget))
		{
			UGameplayStatics::ApplyDamage(InTarget,
				EffectiveDamage,
				Owner ? Owner->GetInstigatorController() : nullptr,
				Owner,
				AttackParams->DamageType);
			SpawnNiagaraEffect(AttackParams->ImpactEffect, InTarget->GetActorTransform());
		}
		return;
	}

	TArray<AActor*> Targets;
	Targets.Reserve(8);
	Targets.Add(InTarget);
	AppendCleaveTargets(InTarget, Targets);

	for (AActor* Target : Targets)
	{
		if (!IsValid(Target))
		{
			continue;
		}
		UGameplayStatics::ApplyDamage(Target,
			EffectiveDamage,
			Owner ? Owner->GetInstigatorController() : nullptr,
			Owner,
			AttackParams->DamageType);
		SpawnNiagaraEffect(AttackParams->ImpactEffect, Target->GetActorTransform());
	}
}

void UCppMeleeAttackBehavior::AppendCleaveTargets(AActor* InPrimaryTarget, TArray<AActor*>& InOutTargets) const
{
	FCppMeleeAttackParams* AttackParams = GetParamsAs<FCppMeleeAttackParams>();
	if (!Owner || AttackParams->CleaveRadius <= 0.f)
	{
		return;
	}

	TScriptInterface<ICppEntity> OwnerEntity = Owner;
	if (!OwnerEntity.GetInterface())
	{
		return;
	}

	const TArray<TEnumAsByte<ECollisionChannel>> TargetTypes{ ECC_Unit, ECC_Building };
	TArray<AActor*> Found = UCppGameplayStatics::FindActors(Owner,
		AttackParams->CleaveRadius,
		TargetTypes,
		AActor::StaticClass(),
		{ Owner },
		Owner->GetActorLocation());

	for (AActor* Actor : Found)
	{
		if (!IsValid(Actor) || Actor == InPrimaryTarget || InOutTargets.Contains(Actor))
		{
			continue;
		}

		TScriptInterface<ICppEntity> Entity = Actor;
		if (!Entity.GetInterface() || !Entity->IsHostileTo(OwnerEntity))
		{
			continue;
		}

		InOutTargets.Add(Actor);
	}
}
