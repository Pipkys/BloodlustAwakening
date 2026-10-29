#pragma once

#include "CoreMinimal.h"

#include "CppCell.generated.h"

class ACppCellGrid;

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API ACppCell : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FIntPoint Coords;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bIsOccupied;

public:
	UFUNCTION(BlueprintCallable, Category = "Cell")
	ACppCellGrid* GetGrid();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Cell")
	void MarkCell();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Cell")
	void UnmarkCell();
};
