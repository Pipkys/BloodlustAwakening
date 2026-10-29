#include "BloodlustAwakening/Components/AttackComponent/Ranged/CppRangedAttackComponent.h"

#include "BloodlustAwakening/Actors/Interface/CppEntity.h"
#include "BloodlustAwakening/Actors/Projectiles/CppProjectile.h"
#include "BloodlustAwakening/Actors/Projectiles/CppProjectileSpawnParameters.h"
#include "BloodlustAwakening/Components/AttackComponent/Ranged/CppRangedAttackComponentStats.h"
#include "BloodlustAwakening/GameMode/CppBloodlustAwakeningGameplayStatics.h"

#include "NiagaraFunctionLibrary.h"

void UCppRangedAttackComponent::SetupComponent(AActor* InOwner, const FCppActorSpawnParameters& InSpawnParameters)
{
	AttackType = ECppAttackType::Ranged;
	Super::SetupComponent(InOwner, InSpawnParameters);
	FCppStats* RangedAttackStats = ((TScriptInterface<ICppEntity>)InOwner)->GetStats(ECppStatsType::RangedAttack);
	FCppRangedAttackComponentStats* Stats = (FCppRangedAttackComponentStats*)RangedAttackStats;
	ProjectileClass = Stats->ProjectileClass;
	bProjectileRotationFollowsVelocity = Stats->bProjectileRotationFollowsVelocity;
	ProjectileGravityScale = Stats->ProjectileGravityScale;
	bIsHomingProjectile = Stats->bIsHomingProjectile;
	ProjectileHomingAccelerationMagnitude = Stats->ProjectileHomingAccelerationMagnitude;
	ProjectileInitialSpeed = Stats->ProjectileInitialSpeed;
	ProjectileMaxSpeed = Stats->ProjectileMaxSpeed;
	ProjectileOverlapRadius = Stats->ProjectileOverlapRadius;
}

void UCppRangedAttackComponent::PerformAttack(AActor* InTarget, const FTransform& InAttackPointTransform)
{
	if (IsValid(AttackEffect))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
			AttackEffect,
			InAttackPointTransform.GetLocation(),
			InAttackPointTransform.GetRotation().Rotator(),
			InAttackPointTransform.GetScale3D());
	}
	FCppProjectileSpawnParameters ProjectileSpawnParameters;
	ProjectileSpawnParameters.Owner = GetOwner();
	ProjectileSpawnParameters.Instigator = GetOwner()->GetInstigator();
	ACppProjectile* Projectile = UCppBloodlustAwakeningGameplayStatics::SpawnProjectile(GetWorld(), 
		ProjectileClass,
		InAttackPointTransform,
		ProjectileSpawnParameters);
	if (Projectile)
	{
		Projectile->LockOnTarget(InTarget);
	}
	OnPerformAttack();
}
