#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"

#include "CppStats.generated.h"

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppStatsDatabaseRow : public FTableRowBase
{
	GENERATED_BODY()
};

using FCppStats = FCppStatsDatabaseRow;

UENUM(BlueprintType)
enum class ECppStatsType : uint8
{
	Unit = 0 UMETA(DisplayName = "Unit"),
	Tower UMETA(DisplayName = "Tower"),
	Health UMETA(DisplayName = "Health"),
	MeleeAttack UMETA(DisplayName = "MeleeAttack"),
	RangedAttack UMETA(DisplayName = "RangedAttack")
};

UENUM(BlueprintType)
enum class ECppEntityStatsType : uint8
{
	Unit = 0 UMETA(DisplayName = "Unit"),
	Tower UMETA(DisplayName = "Tower"),
};

UENUM(BlueprintType)
enum class ECppComponentStatsType : uint8
{
	Health UMETA(DisplayName = "Health"),
	MeleeAttack UMETA(DisplayName = "MeleeAttack"),
	RangedAttack UMETA(DisplayName = "RangedAttack")
};

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppStatsDatabaseRowView
{
	GENERATED_BODY()

public:
	UClass* ClassBoundedTo;
	ECppStatsType Type;
	const UDataTable* Database;

public:
	static FCppStats* Load(UClass* InClass, const UDataTable* InDatabase);
	static FCppStats* Load(UObject* InObject, const UDataTable* InDatabase);
	FCppStats* Load() const;
};

using FCppStatsView = FCppStatsDatabaseRowView;
