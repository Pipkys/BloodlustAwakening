#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Core/Stats/CppStats.h"

#include "CppUnitStats.generated.h"

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppUnitStatsDatabaseRow : public FCppStatsDatabaseRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	float DetectionRange;
};

using FCppUnitStats = FCppUnitStatsDatabaseRow;
