#pragma once

#include "CoreMinimal.h"

#include "GameFramework/DamageType.h"

#include "BloodlustAwakening/Core/SpawnParameters/CppActorSpawnParameters.h"

#include "CppProjectileSpawnParameters.generated.h"

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppProjectileSpawnParameters : public FCppActorSpawnParameters
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<UNiagaraSystem> ShotEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float Damage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	bool bRotationFollowsVelocity = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float GravityScale = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	bool bIsHoming = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float HomingAccelerationMagnitude = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float InitialSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float MaxSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float OverlapRadius = 4.f;
};
