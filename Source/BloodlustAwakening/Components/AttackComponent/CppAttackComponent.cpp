#include "BloodlustAwakening/Components/AttackComponent/CppAttackComponent.h"

#include "BloodlustAwakening/Components/AttackComponent/CppAttackStats.h"
#include "BloodlustAwakening/Components/StatsComponent/CppStatsComponent.h"
#include "BloodlustAwakening/Core/Physics/CppCollisionChannels.h"
#include "BloodlustAwakening/Core/Stats/CppStats.h"
#include "BloodlustAwakening/GameMode/CppBloodlustAwakeningGameplayStatics.h"
#include "BloodlustAwakening/Interface/CppEntity.h"
#include "BloodlustAwakening/Projectiles/CppProjectile.h"
#include "BloodlustAwakening/Projectiles/CppProjectileSpawnParameters.h"
#include "BloodlustAwakening/Subsystems/CppStatsSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

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
	AttackType = Stats->AttackType;
	DamageType = Stats->DamageType;
	AttackEffect = Stats->AttackEffect;
	ImpactEffect = Stats->ImpactEffect;
	Damage = Stats->Damage;
	Speed = Stats->Speed;
	Range = Stats->Range;
	bSingleTarget = Stats->bSingleTarget;
	CleaveRadius = Stats->CleaveRadius;
	ProjectileClass = Stats->ProjectileClass;
	ShotEffect = Stats->ShotEffect;
	bProjectileRotationFollowsVelocity = Stats->bProjectileRotationFollowsVelocity;
	ProjectileGravityScale = Stats->ProjectileGravityScale;
	bIsHomingProjectile = Stats->bIsHomingProjectile;
	ProjectileHomingAccelerationMagnitude = Stats->ProjectileHomingAccelerationMagnitude;
	ProjectileInitialSpeed = Stats->ProjectileInitialSpeed;
	ProjectileMaxSpeed = Stats->ProjectileMaxSpeed;
	ProjectileOverlapRadius = Stats->ProjectileOverlapRadius;
}

float UCppAttackComponent::GetDamage() const
{
	return Damage;
}

float UCppAttackComponent::GetSpeed() const
{
	return Speed;
}

float UCppAttackComponent::GetRange() const
{
	return Range;
}

bool UCppAttackComponent::IsTargetInAttackRange(AActor* InTarget, const FTransform& InPointTransform) const
{
	const float AttackRange = GetRange();
	return FVector::DistSquared(InPointTransform.GetLocation(), InTarget->GetActorLocation()) <= AttackRange * AttackRange;
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

void UCppAttackComponent::PerformAttack(AActor* InTarget, const FTransform& InAttackPointTransform)
{
	switch (AttackType)
	{
		case ECppAttackType::Melee:
			PerformMeleeAttack(InTarget, InAttackPointTransform);
			break;
		case ECppAttackType::Ranged:
			PerformRangedAttack(InTarget, InAttackPointTransform);
			break;
		default:
			checkNoEntry();
			break;
	}
	StartAttackCooldown();
}

void UCppAttackComponent::SetAttackTimerCallback(UObject* InObject, const FName& InFunctionName)
{
	FScriptDelegate Delegate;
	Delegate.BindUFunction(InObject, InFunctionName);
	AttackTimerCallback.AddUnique(Delegate);
}

void UCppAttackComponent::PerformMeleeAttack(AActor* InTarget, const FTransform& InAttackPointTransform)
{
	AActor* Owner = GetOwner();
	const float EffectiveDamage = GetDamage();
	SpawnNiagaraEffect(AttackEffect, InAttackPointTransform);
	if (bSingleTarget)
	{
		if (IsValid(InTarget))
		{
			UGameplayStatics::ApplyDamage(InTarget,
				EffectiveDamage,
				Owner ? Owner->GetInstigatorController() : nullptr,
				Owner,
				DamageType);
			SpawnNiagaraEffect(ImpactEffect, InTarget->GetActorTransform());
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
			DamageType);
		SpawnNiagaraEffect(ImpactEffect, Target->GetActorTransform());
	}
}

void UCppAttackComponent::PerformRangedAttack(AActor* InTarget, const FTransform& InAttackPointTransform)
{
	if (!ProjectileClass)
	{
		UE_LOG(LogTemp, Error, TEXT("UCppAttackComponent::PerformRangedAttack: projectile class is not set on '%s'"), *GetNameSafe(GetOwner()));
		return;
	}
	SpawnNiagaraEffect(AttackEffect, InAttackPointTransform);
	AActor* Owner = GetOwner();
	FCppProjectileSpawnParameters ProjectileSpawnParameters;
	ProjectileSpawnParameters.Owner = Owner;
	ProjectileSpawnParameters.Instigator = Owner ? Owner->GetInstigator() : nullptr;
	if (!ProjectileSpawnParameters.Instigator)
	{
		ProjectileSpawnParameters.Instigator = Cast<APawn>(Owner);
	}
	ProjectileSpawnParameters.DamageType = DamageType;
	ProjectileSpawnParameters.ShotEffect = ShotEffect;
	ProjectileSpawnParameters.ImpactEffect = ImpactEffect;
	ProjectileSpawnParameters.Damage = GetDamage();
	ProjectileSpawnParameters.bRotationFollowsVelocity = bProjectileRotationFollowsVelocity;
	ProjectileSpawnParameters.GravityScale = ProjectileGravityScale;
	ProjectileSpawnParameters.bIsHoming = bIsHomingProjectile;
	ProjectileSpawnParameters.HomingAccelerationMagnitude = ProjectileHomingAccelerationMagnitude;
	ProjectileSpawnParameters.InitialSpeed = ProjectileInitialSpeed;
	ProjectileSpawnParameters.MaxSpeed = ProjectileMaxSpeed;
	ProjectileSpawnParameters.OverlapRadius = ProjectileOverlapRadius;
	ACppProjectile* Projectile = UCppBloodlustAwakeningGameplayStatics::SpawnProjectile(GetWorld(),
		ProjectileClass,
		InAttackPointTransform,
		ProjectileSpawnParameters);
	if (Projectile)
	{
		Projectile->LockOnTarget(InTarget);
	}
}

void UCppAttackComponent::AppendCleaveTargets(AActor* InPrimaryTarget, TArray<AActor*>& InOutTargets) const
{
	AActor* Owner = GetOwner();
	if (!Owner || CleaveRadius <= 0.f)
	{
		return;
	}
	TScriptInterface<ICppEntity> OwnerEntity = Owner;
	if (!OwnerEntity.GetInterface())
	{
		return;
	}
	const TArray<TEnumAsByte<ECollisionChannel>> TargetTypes{ ECC_Unit, ECC_Building };
	TArray<AActor*> Found = UCppBloodlustAwakeningGameplayStatics::FindActors(Owner,
		CleaveRadius,
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

void UCppAttackComponent::StartAttackCooldown()
{
	GetWorld()->GetTimerManager().SetTimer(AttackTimer,
		FTimerDelegate::CreateUObject(this, &UCppAttackComponent::OnAttackCooldownFinished),
		GetSpeed(),
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

void UCppAttackComponent::SpawnNiagaraEffect(UNiagaraSystem* InEffect, const FTransform& InTransform) const
{
	if (IsValid(InEffect))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
			InEffect,
			InTransform.GetLocation(),
			InTransform.GetRotation().Rotator(),
			InTransform.GetScale3D());
	}
}
