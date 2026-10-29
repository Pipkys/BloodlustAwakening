#include "BloodlustAwakening/Level/Grid/CppTileGrid.h"

FIntPoint ACppTileGrid::WorldToGrid(const FVector& InWorldLocation) const
{
	const FVector Offset = InWorldLocation - GetActorLocation();
	const int32 CoordX = FMath::RoundToInt(Offset.X / CellSize);
	const int32 CoordY = FMath::RoundToInt(Offset.Y / CellSize);
	return FIntPoint(CoordX, CoordY);
}

FVector ACppTileGrid::GridToWorld(int32 InCoordX, int32 InCoordY) const
{
	const float CoordX = InCoordX * CellSize;
	const float CoordY = InCoordY * CellSize;
	return GetActorLocation() + FVector(CoordX, CoordY, 0.f);
}
