#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"

#include "BloodlustAwakening/Core/Stats/CppStats.h"

#include "CppHealthStats.generated.h"

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppHealthStatsDatabaseRow : public FCppStatsDatabaseRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 0.f;
};

using FCppHealthStats = FCppHealthStatsDatabaseRow;

template<>
struct TCppStatsTypeTraits<ECppStatsType::Health>
{
	using Type = FCppHealthStats;
};

