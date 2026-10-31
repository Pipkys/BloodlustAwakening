#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"

#include "BloodlustAwakening/Buildings/Implementation/CppBuildingStats.h"
#include "BloodlustAwakening/Components/AttackComponent/CppAttackStats.h"
#include "BloodlustAwakening/Components/HealthComponent/CppHealthStats.h"
#include "BloodlustAwakening/Core/Stats/CppStats.h"

#include "CppTowerStats.generated.h"

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppTowerStatsDatabaseRow : public FCppStatsDatabaseRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	FCppHealthStatsDatabaseRow Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FCppAttackStatsDatabaseRow Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower")
	FCppBuildingStatsDatabaseRow Building;
};

using FCppTowerStats = FCppTowerStatsDatabaseRow;

template<>
struct TCppStatsTypeTraits<ECppStatsType::Tower> 
{ 
	using Type = FCppTowerStats; 
};
