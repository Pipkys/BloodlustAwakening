#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "BloodlustAwakening/Level/Grid/CppCell.h"
#include "BloodlustAwakening/Level/Grid/CppCellGrid.h"

#include "CppCellGridGenerator.generated.h"

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppCircleGridData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TObjectPtr<ACppCellGrid> CellGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 Radius;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FCppOnCellSpawnedEvent, ACppCell*, InCell);

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppRectangleGridData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TObjectPtr<ACppCellGrid> CellGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 RowCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 ColumnCount;
};

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API ACppCellGridGenerator : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Generation")
	virtual FCppCircleGridData GenerateCircleGrid(TSubclassOf<ACppCell> InCellClass, 
		TSubclassOf<ACppCellGrid> InCellGridClass,
		float InCellSize, 
		int32 InRadius,
		const FCppOnCellSpawnedEvent& InOnCellSpawnEvent)
	{ 
		PURE_VIRTUAL(ACppCellGridGenerator::GenerateCircleGrid) return FCppCircleGridData();
	};

	UFUNCTION(BlueprintCallable, Category = "Generation")
	virtual FCppRectangleGridData GenerateRectangleGrid(TSubclassOf<ACppCell> InCellClass, 
		TSubclassOf<ACppCellGrid> InCellGridClass,
		float InCellSize,
		int32 InRowCount, 
		int32 InColumnCount,
		const FCppOnCellSpawnedEvent& InOnCellSpawnEvent)
	{ 
		PURE_VIRTUAL(ACppCellGridGenerator::GenerateRectangleGrid) return FCppRectangleGridData(); 
	};
};
