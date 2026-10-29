#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Level/Grid/CppCellGrid.h"

#include "CppTileGrid.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API ACppTileGrid : public ACppCellGrid
{
	GENERATED_BODY()

public:
	FIntPoint WorldToGrid(const FVector& InWorldLocation) const override;
	FVector GridToWorld(int32 InCoordX, int32 InCoordY) const override;
};
