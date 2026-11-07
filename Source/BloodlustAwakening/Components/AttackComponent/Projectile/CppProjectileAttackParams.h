#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Components/AttackComponent/CppAttackParams.h"

#include "CppProjectileAttackParams.generated.h"

class ACppProjectile;

USTRUCT(BlueprintType)
struct FCppProjectileAttackParams : public FCppAttackParams
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Projectile")
	TSubclassOf<ACppProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Projectile")
	TObjectPtr<UNiagaraSystem> ShotEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Projectile")
	float InitialSpeed = 1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Projectile")
	float MaxSpeed = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Projectile")
	float GravityScale = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Projectile")
	bool bRotationFollowsVelocity = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Projectile")
	bool bIsHoming = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Projectile", Meta = (EditCondition = "bIsHoming", ClampMin = "0"))
	float HomingAccelerationMagnitude = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Projectile")
	float OverlapRadius = 4.f;
};
