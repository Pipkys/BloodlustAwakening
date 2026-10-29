#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Core/Stats/CppStats.h"

#include "CppBuildingStats.generated.h"

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppBuildingStatsDatabaseRow : public FCppStatsDatabaseRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NotUsed")
	float NotUsed;
};

using FCppBuildingStats = FCppBuildingStatsDatabaseRow;
