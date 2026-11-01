#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "GameFramework/DamageType.h"
#include "NiagaraSystem.h"

#include "BloodlustAwakening/Components/AttackComponent/CppAttackType.h"
#include "BloodlustAwakening/Core/Stats/CppStats.h"

#include "CppAttackStats.generated.h"

class ACppProjectile;

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppAttackStatsDatabaseRow : public FCppStatsDatabaseRow
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
	float ProjectileGravityScale = 0.0f;

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

public:
	inline TArray<TPair<ECppStatsType, FCppStats*>> GetStatsSections() override
	{
		return {};
	}
};

using FCppAttackStats = FCppAttackStatsDatabaseRow;

template<>
struct TCppStatsTypeTraits<ECppStatsType::Attack>
{
	using Type = FCppAttackStats;
};
