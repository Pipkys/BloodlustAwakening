#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "GameFramework/DamageType.h"
#include "NiagaraSystem.h"

#include "BloodlustAwakening/Components/AttackComponent/CppAttackType.h"
#include "BloodlustAwakening/Core/SpawnParameters/CppActorSpawnParameters.h"

#include "CppAttackComponent.generated.h"

class ACppProjectile;

UCLASS(BlueprintType, Blueprintable, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	ECppAttackType AttackType = ECppAttackType::Melee;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TObjectPtr<UNiagaraSystem> AttackEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Damage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Speed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Range = 0.f;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Melee")
	bool bSingleTarget = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Melee")
	float CleaveRadius = 0.f;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Ranged|Projectile")
	TSubclassOf<ACppProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Ranged|Projectile")
	TObjectPtr<UNiagaraSystem> ShotEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Ranged|Projectile")
	bool bProjectileRotationFollowsVelocity = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Ranged|Projectile")
	float ProjectileGravityScale = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Ranged|Projectile")
	bool bIsHomingProjectile = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Ranged|Projectile")
	float ProjectileHomingAccelerationMagnitude = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Ranged|Projectile")
	float ProjectileInitialSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Ranged|Projectile")
	float ProjectileMaxSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Ranged|Projectile")
	float ProjectileOverlapRadius = 4.f;

private:
	FTimerHandle AttackTimer;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackTimerCallback);
	UPROPERTY()
	FAttackTimerCallback AttackTimerCallback = {};

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void SetupComponent(AActor* InOwner, const FCppActorSpawnParameters& InSpawnParameters);

public:
	UFUNCTION(BlueprintCallable, Category = "Attack")
	float GetDamage() const;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	float GetSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	float GetRange() const;

public:
	UFUNCTION(BlueprintCallable, Category = "Attack")
	bool IsTargetInAttackRange(AActor* InTarget, const FTransform& InAttackPointTransform) const;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	bool IsCanAttack() const;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	bool IsCanAttackTarget(AActor* InTarget, const FTransform& InAttackPointTransform) const;

public:
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void PerformAttack(AActor* InTarget, const FTransform& InAttackPointTransform);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SetAttackTimerCallback(UObject* InObject, const FName& InFunctionName);

protected:
	void PerformMeleeAttack(AActor* InTarget, const FTransform& InAttackPointTransform);
	void PerformRangedAttack(AActor* InTarget, const FTransform& InAttackPointTransform);

	void AppendCleaveTargets(AActor* InPrimaryTarget, TArray<AActor*>& InOutTargets) const;

	void StartAttackCooldown();
	void OnAttackCooldownFinished();

	void SpawnNiagaraEffect(UNiagaraSystem* InEffect, const FTransform& InTransform) const;
};
