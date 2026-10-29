#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "BloodlustAwakening/Actors/Projectiles/CppProjectile.h"
#include "BloodlustAwakening/Components/AttackComponent/CppAttackComponent.h"
#include "BloodlustAwakening/Components/AttackComponent/CppAttackComponentStats.h"
#include "BloodlustAwakening/Core/Stats/CppStats.h"

#include "CppRangedAttackComponent.generated.h"

UCLASS(BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppRangedAttackComponent : public UCppAttackComponent
{
	GENERATED_BODY()

	using Super = UCppAttackComponent;

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

public:
	void SetupComponent(AActor* InOwner, const FCppActorSpawnParameters& InSpawnParameters) override;

public:
	void PerformAttack(AActor* InTarget, const FTransform& InAttackPointTransform) override;
};
