#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Level/Grid/CppCellGrid.h"

#include "CppHexGrid.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API ACppHexGrid : public ACppCellGrid
{
	GENERATED_BODY()

public:
	FIntPoint WorldToGrid(const FVector& InWorldLocation) const override;
	FVector GridToWorld(int32 InCoordQ, int32 InCoordR) const override;

protected:
	FVector CalculateHexLocation(int32 InCoordQ, int32 InCoordR) const;
};
