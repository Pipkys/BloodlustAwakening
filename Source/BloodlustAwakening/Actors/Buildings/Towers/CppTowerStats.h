#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Actors/Buildings/CppBuildingStats.h"

#include "CppTowerStats.generated.h"

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppTowerStatsDatabaseRow : public FCppBuildingStatsDatabaseRow
{
	GENERATED_BODY()

public:

};

using FCppTowerStats = FCppTowerStatsDatabaseRow;
