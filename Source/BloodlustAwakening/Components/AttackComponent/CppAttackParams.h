#pragma once

#include "CoreMinimal.h"

#include "StructUtils/InstancedStruct.h"

#include "CppAttackParams.generated.h"

class UCppAttackBehavior;
class UNiagaraSystem;
class UDamageType;
class ACppProjectile;

USTRUCT(BlueprintType)
struct FCppAttackParams
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<UCppAttackBehavior> AttackBehavior;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Range = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Speed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TObjectPtr<UNiagaraSystem> AttackEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TObjectPtr<UNiagaraSystem> ImpactEffect;
};

//USTRUCT(BlueprintType)
//struct FCppMeleeAttackParams : public FCppAttackParams
//{
//	GENERATED_BODY()
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Melee")
//	bool bSingleTarget = true;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Melee", Meta = (EditCondition = "!bSingleTarget", ClampMin = "0"))
//	float CleaveRadius = 150.f;
//};
//
//USTRUCT(BlueprintType)
//struct FCppProjectileAttackParams : public FCppAttackParams
//{
//	GENERATED_BODY()
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Projectile")
//	TSubclassOf<ACppProjectile> ProjectileClass;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Projectile")
//	float InitialSpeed = 1200.f;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Projectile")
//	float MaxSpeed = 2000.f;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Projectile")
//	float GravityScale = 0.f;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Projectile")
//	bool bRotationFollowsVelocity = false;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Projectile")
//	bool bIsHoming = true;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Projectile", Meta = (EditCondition = "bIsHoming", ClampMin = "0"))
//	float HomingAcceleration = 2000.f;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Projectile")
//	float OverlapRadius = 4.f;
//};
//
//USTRUCT(BlueprintType)
//struct FCppLaserAttackParams : public FCppAttackParams
//{
//	GENERATED_BODY()
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laser")
//	float BeamDuration = 0.2f;
//};
//
//USTRUCT(BlueprintType)
//struct FCppFlamethrowerAttackParams : public FCppAttackParams
//{
//	GENERATED_BODY()
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flamethrower", Meta = (ClampMin = "1", ClampMax = "180"))
//	float ConeAngle = 45.f;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flamethrower")
//	float TickInterval = 0.1f;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flamethrower")
//	float TickDamage = 2.f;
//};
