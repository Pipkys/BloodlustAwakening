#include "BloodlustAwakening/Components/AttackComponent/Projectile/CppProjectileAttackBehavior.h"

#include "BloodlustAwakening/Components/AttackComponent/Projectile/CppProjectileAttackParams.h"
#include "BloodlustAwakening/GameMode/CppGameplayStatics.h"
#include "BloodlustAwakening/Projectiles/CppProjectileSpawnParameters.h"
#include "BloodlustAwakening/Projectiles/CppProjectile.h"

#include "NiagaraFunctionLibrary.h"

void UCppProjectileAttackBehavior::PerformAttack(AActor* InTarget, const FTransform& InAttackPoint)
{
	FCppProjectileAttackParams* AttackParams = GetParamsAs<FCppProjectileAttackParams>();
	if (!AttackParams->ProjectileClass)
	{
		UE_LOG(LogTemp, Error, TEXT("UCppAttackComponent::PerformRangedAttack: projectile class is not set on '%s'"), *GetNameSafe(Owner));
		return;
	}

	SpawnNiagaraEffect(AttackParams->AttackEffect, InAttackPoint);
	FCppProjectileSpawnParameters ProjectileSpawnParameters;
	ProjectileSpawnParameters.Owner = Owner;
	ProjectileSpawnParameters.Instigator = Owner ? Owner->GetInstigator() : nullptr;
	if (!ProjectileSpawnParameters.Instigator)
	{
		ProjectileSpawnParameters.Instigator = Cast<APawn>(Owner);
	}
	ProjectileSpawnParameters.DamageType = AttackParams->DamageType;
	ProjectileSpawnParameters.ShotEffect = AttackParams->ShotEffect;
	ProjectileSpawnParameters.ImpactEffect = AttackParams->ImpactEffect;
	ProjectileSpawnParameters.Damage = AttackParams->Damage;
	ProjectileSpawnParameters.bRotationFollowsVelocity = AttackParams->bRotationFollowsVelocity;
	ProjectileSpawnParameters.GravityScale = AttackParams->GravityScale;
	ProjectileSpawnParameters.bIsHoming = AttackParams->bIsHoming;
	ProjectileSpawnParameters.HomingAccelerationMagnitude = AttackParams->HomingAccelerationMagnitude;
	ProjectileSpawnParameters.InitialSpeed = AttackParams->InitialSpeed;
	ProjectileSpawnParameters.MaxSpeed = AttackParams->MaxSpeed;
	ProjectileSpawnParameters.OverlapRadius = AttackParams->OverlapRadius;

	ACppProjectile* Projectile = UCppGameplayStatics::SpawnProjectile(GetWorld(),
		AttackParams->ProjectileClass,
		InAttackPoint,
		ProjectileSpawnParameters);
	if (Projectile)
	{
		Projectile->LockOnTarget(InTarget);
	}
}
