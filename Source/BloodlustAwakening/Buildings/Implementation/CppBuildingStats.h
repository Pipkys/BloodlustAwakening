#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"

#include "BloodlustAwakening/Core/Stats/CppStats.h"

#include "CppBuildingStats.generated.h"

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppBuildingStatsDatabaseRow : public FCppStatsDatabaseRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	int32 SizeInCells = 1;
};

using FCppBuildingStats = FCppBuildingStatsDatabaseRow;

template<>
struct TCppStatsTypeTraits<ECppStatsType::Building>
{
	using Type = FCppBuildingStats;
};
