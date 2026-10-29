#pragma once

#include "CoreMinimal.h"

#include "NiagaraSystem.h"

#include "BloodlustAwakening/Core/Stats/CppStats.h"

#include "CppAttackComponentStats.generated.h"

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppAttackComponentStatsDatabaseRow : public FCppStatsDatabaseRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TObjectPtr<UNiagaraSystem> AttackEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Range;
};

using FCppAttackComponentStats = FCppAttackComponentStatsDatabaseRow;
