#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Level/Grid/CppCell.h"

#include "CppCellGrid.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API ACppCellGrid : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TMap<FIntPoint, TObjectPtr<ACppCell>> Cells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	float CellSize;

public:
	UFUNCTION(BlueprintCallable, Category = "Transformation")
	virtual FIntPoint WorldToGrid(const FVector& InWorldLocation) const 
	{ 
		PURE_VIRTUAL(ACppCellGrid::WorldToGrid) return FIntPoint(); 
	}

	UFUNCTION(BlueprintCallable, Category = "Transformation")
	virtual FVector GridToWorld(int32 InCoordX, int32 InCoordY) const 
	{ 
		PURE_VIRTUAL(ACppCellGrid::GridToWorld) return FVector (); 
	}
};
