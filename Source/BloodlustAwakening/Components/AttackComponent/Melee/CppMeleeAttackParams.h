#pragma once

#include "CoreMinimal.h"

#include "StructUtils/InstancedStruct.h"

#include "BloodlustAwakening/Components/AttackComponent/CppAttackParams.h"

#include "CppMeleeAttackParams.generated.h"

USTRUCT(BlueprintType)
struct FCppMeleeAttackParams : public FCppAttackParams
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Melee")
	bool bSingleTarget = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Melee", Meta = (EditCondition = "!bSingleTarget", ClampMin = "0"))
	float CleaveRadius = 150.f;
};