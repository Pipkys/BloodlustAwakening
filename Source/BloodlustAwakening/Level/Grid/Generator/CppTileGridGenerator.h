
#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Level/Grid/Generator/CppCellGridGenerator.h"

#include "CppTileGridGenerator.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API ACppTileGridGenerator : public ACppCellGridGenerator
{
	GENERATED_BODY()

public:
	FCppCircleGridData GenerateCircleGrid(TSubclassOf<ACppCell> InCellClass,
		TSubclassOf<ACppCellGrid> InCellGridClass,
		float InCellSize,
		int32 InRadius,
		const FCppOnCellSpawnedEvent& InOnCellSpawnEvent) override;

	FCppRectangleGridData GenerateRectangleGrid(TSubclassOf<ACppCell> InCellClass,
		TSubclassOf<ACppCellGrid> InCellGridClass,
		float InCellSize,
		int32 InRowCount,
		int32 InColumnCount,
		const FCppOnCellSpawnedEvent& InOnCellSpawnEvent) override;
};
