#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"

#include "BloodlustAwakening/Components/AttackComponent/CppAttackStats.h"
#include "BloodlustAwakening/Components/HealthComponent/CppHealthStats.h"
#include "BloodlustAwakening/Core/Stats/CppStats.h"

#include "CppUnitStats.generated.h"

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppUnitStatsDatabaseRow : public FCppStatsDatabaseRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	FCppHealthStatsDatabaseRow Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FCppAttackStatsDatabaseRow Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	float DetectionRange = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxWalkSpeed = 0.f;

public:
	inline TArray<TPair<ECppStatsType, FCppStats*>> GetStatsSections() override
	{
		return
		{
			{ ECppStatsType::Health, &Health },
			{ ECppStatsType::Attack, &Attack }
		};
	}
};

using FCppUnitStats = FCppUnitStatsDatabaseRow;

template<>
struct TCppStatsTypeTraits<ECppStatsType::Unit>
{
	using Type = FCppUnitStats;
};
