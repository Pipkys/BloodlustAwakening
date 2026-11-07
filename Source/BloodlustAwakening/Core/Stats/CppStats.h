#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"

#include "CppStats.generated.h"

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppStatsDatabaseRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	static FName GetArchetypeRowName(const UClass* InClass);
	virtual TArray<TPair<ECppStatsType, FCppStatsDatabaseRow*>> GetStatsSections()
	{
		PURE_VIRTUAL(FCppStatsDatabaseRow::GetStatsSections) return {};
	}
};

using FCppStats = FCppStatsDatabaseRow;

UENUM(BlueprintType)
enum class ECppStatsType : uint8
{
	Health = 0 UMETA(DisplayName = "Health"),
	Attack UMETA(DisplayName = "Attack"),
	Unit UMETA(DisplayName = "Unit"),
	Building UMETA(DisplayName = "Building"),
	Tower UMETA(DisplayName = "Tower")
};

template<ECppStatsType>
struct TCppStatsTypeTraits;
