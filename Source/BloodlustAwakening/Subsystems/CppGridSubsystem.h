#pragma once

#include "CoreMinimal.h"

#include "Subsystems/WorldSubsystem.h"

#include "BloodlustAwakening/Level/Grid/CppCell.h"
#include "BloodlustAwakening/Level/Grid/CppCellGrid.h"
#include "BloodlustAwakening/Level/Grid/CppGridType.h"
#include "BloodlustAwakening/Level/Grid/Generator/CppCellGridGenerator.h"

#include "CppGridSubsystem.generated.h"

USTRUCT()
struct FCppGeneratorData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<ACppCellGridGenerator> Generator;
	TSubclassOf<ACppCellGrid> GridClass;
	TSubclassOf<ACppCell> CellClass;
	float CellSize;
};

UCLASS(BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppGridSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	TMap<ECppGridType, FCppGeneratorData> GeneratorsData;

public:
	UFUNCTION(BlueprintCallable, Category = "Generation")
	void SetupGenerator(ECppGridType InGridType, 
		TSubclassOf<ACppCellGridGenerator> InGeneratorClass, 
		TSubclassOf<ACppCellGrid> InGridClass,
		TSubclassOf<ACppCell> InCellClass, 
		float InCellSize);

public:
	UFUNCTION(BlueprintCallable, Category = "Generation")
	FCppCircleGridData GenerateCircleGrid(ECppGridType InGridType, 
		int32 InRadius,
		const FCppOnCellSpawnedEvent& InOnCellSpawnEvent);

	UFUNCTION(BlueprintCallable, Category = "Generation")
	FCppRectangleGridData GenerateRectangleGrid(ECppGridType InGridType, 
		int32 InRowCount, 
		int32 InColumnCount, 
		const FCppOnCellSpawnedEvent& InOnCellSpawnEvent);
};
