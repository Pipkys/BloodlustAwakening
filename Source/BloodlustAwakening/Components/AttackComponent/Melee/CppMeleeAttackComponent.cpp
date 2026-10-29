#include "BloodlustAwakening/Components/AttackComponent/Melee/CppMeleeAttackComponent.h"

#include "BloodlustAwakening/Actors/Interface/CppEntity.h"
#include "BloodlustAwakening/Actors/Projectiles/CppProjectile.h"
#include "BloodlustAwakening/Components/AttackComponent/Melee/CppMeleeAttackComponentStats.h"

#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

void UCppMeleeAttackComponent::SetupComponent(AActor* InOwner, const FCppActorSpawnParameters& InSpawnParameters)
{
	AttackType = ECppAttackType::Melee;
	Super::SetupComponent(InOwner, InSpawnParameters);
	FCppStats* MeleeAttackStats = ((TScriptInterface<ICppEntity>)InOwner)->GetStats(ECppStatsType::MeleeAttack);
	FCppMeleeAttackComponentStats* Stats = (FCppMeleeAttackComponentStats*)MeleeAttackStats;
	bSingleTarget = Stats->bSingleTarget;
	CleaveRadius = Stats->CleaveRadius;
	ImpactEffect = Stats->ImpactEffect;
}

void UCppMeleeAttackComponent::PerformAttack(AActor* InTarget, const FTransform& InAttackPointTransform)
{
	UGameplayStatics::ApplyDamage(
		InTarget,
		Damage,
		GetOwner()->GetInstigatorController(),
		GetOwner(),
		DamageType);
	auto SpawnEffect = [](UWorld* InWorld, UNiagaraSystem* InEffect, const FTransform& InTransform)
	{
		if (IsValid(InEffect))
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld,
				InEffect,
				InTransform.GetLocation(),
				InTransform.GetRotation().Rotator(),
				InTransform.GetScale3D());
		}
	};
	SpawnEffect(GetWorld(), AttackEffect, InAttackPointTransform);
	SpawnEffect(GetWorld(), ImpactEffect, InTarget->GetActorTransform());
	OnPerformAttack();
}
