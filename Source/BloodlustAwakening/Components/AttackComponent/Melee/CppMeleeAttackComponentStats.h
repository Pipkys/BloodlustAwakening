#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Components/AttackComponent/CppAttackComponentStats.h"

#include "CppMeleeAttackComponentStats.generated.h"

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppMeleeAttackComponentStatsDatabaseRow : public FCppAttackComponentStatsDatabaseRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Melee")
	bool bSingleTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Melee")
	float CleaveRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Melee")
	TObjectPtr<UNiagaraSystem> ImpactEffect;
};

using FCppMeleeAttackComponentStats = FCppMeleeAttackComponentStatsDatabaseRow;
