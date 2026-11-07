#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Core/Stats/CppStats.h"

#include "Engine/DataTable.h"
#include "StructUtils/InstancedStruct.h"

#include "CppAttackStats.generated.h"

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppAttackStatsDatabaseRow : public FCppStatsDatabaseRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (BaseStruct = "/Script/BloodlustAwakening.CppAttackParams"))
	FInstancedStruct AttackParams;

public:
	inline TArray<TPair<ECppStatsType, FCppStats*>> GetStatsSections() override
	{
		return {};
	}
};

using FCppAttackStats = FCppAttackStatsDatabaseRow;

template<>
struct TCppStatsTypeTraits<ECppStatsType::Attack>
{
	using Type = FCppAttackStats;
};
