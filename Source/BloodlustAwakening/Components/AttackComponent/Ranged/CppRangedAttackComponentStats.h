#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Actors/Projectiles/CppProjectile.h"
#include "BloodlustAwakening/Components/AttackComponent/CppAttackComponentStats.h"

#include "CppRangedAttackComponentStats.generated.h"

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppRangedAttackComponentStatsDatabaseRow : public FCppAttackComponentStatsDatabaseRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Ranged|Projectile")
	TSubclassOf<ACppProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Ranged|Projectile")
	bool bProjectileRotationFollowsVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Ranged|Projectile")
	float ProjectileGravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Ranged|Projectile")
	bool bIsHomingProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Ranged|Projectile")
	float ProjectileHomingAccelerationMagnitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Ranged|Projectile")
	float ProjectileInitialSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Ranged|Projectile")
	float ProjectileMaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Ranged|Projectile")
	float ProjectileOverlapRadius;
};

using FCppRangedAttackComponentStats = FCppRangedAttackComponentStatsDatabaseRow;
