#include "BloodlustAwakening/Level/Grid/Generator/CppTileGridGenerator.h"

#include "BloodlustAwakening/Level/Grid/CppTileGrid.h"

#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

FCppCircleGridData ACppTileGridGenerator::GenerateCircleGrid(TSubclassOf<ACppCell> InCellClass,
	TSubclassOf<ACppCellGrid> InCellGridClass,
	float InCellSize,
	int32 InRadius,
	const FCppOnCellSpawnedEvent& InOnCellSpawnEvent)
{
	FCppCircleGridData GridData;
	TObjectPtr<ACppCellGrid>& TileGrid = GridData.CellGrid;
	TileGrid = nullptr;
	GridData.Radius = InRadius;
	if (InRadius < 1)
	{
		return GridData;
	}

	FActorSpawnParameters TileGridSpawnParameters;
	TileGridSpawnParameters.Owner = this;
	TileGridSpawnParameters.Instigator = GetInstigator();
	TileGridSpawnParameters.bNoFail = true;
	TileGrid = GetWorld()->SpawnActor<ACppTileGrid>(InCellGridClass, TileGridSpawnParameters);
	TileGrid->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	TileGrid->CellSize = InCellSize;
	TMap<FIntPoint, TObjectPtr<ACppCell>>& GridTiles = TileGrid->Cells;

	const float GridRadiusSqr = InRadius * InRadius;
	for (int32 CoordX = -InRadius; CoordX <= InRadius; ++CoordX)
	{
		for (int32 CoordY = -InRadius; CoordY <= InRadius; ++CoordY)
		{
			const FVector TileLocation = GetActorLocation() + FVector(CoordX * InCellSize, CoordY * InCellSize, 0.f);
			const FVector Offset = TileLocation - GetActorLocation();
			const float DistSqr = (CoordX * CoordX) + (CoordY * CoordY);
			if (DistSqr <= GridRadiusSqr)
			{
				FActorSpawnParameters TileSpawnParameters;
				TileSpawnParameters.Owner = TileGrid;
				TileSpawnParameters.Instigator = TileGrid->GetInstigator();
				TileSpawnParameters.bNoFail = true;
				ACppCell* Tile = GetWorld()->SpawnActor<ACppCell>(InCellClass, TileLocation, FRotator::ZeroRotator, TileSpawnParameters);
				Tile->SetActorLabel(FString::Printf(TEXT("Tile_%d_%d"), CoordX, CoordY));
				Tile->AttachToActor(TileGrid, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
				Tile->Coords = FIntPoint(CoordX, CoordY);
				Tile->bIsOccupied = false;
				GridTiles.Add(Tile->Coords, Tile);
				InOnCellSpawnEvent.Execute(Tile);
			}
		}
	}
	return GridData;
}

FCppRectangleGridData ACppTileGridGenerator::GenerateRectangleGrid(TSubclassOf<ACppCell> InCellClass,
	TSubclassOf<ACppCellGrid> InCellGridClass,
	float InCellSize,
	int32 InRowCount,
	int32 InColumnCount,
	const FCppOnCellSpawnedEvent& InOnCellSpawnEvent)
{
	FCppRectangleGridData GridData;
	TObjectPtr<ACppCellGrid>& TileGrid = GridData.CellGrid;
	TileGrid = nullptr;
	GridData.RowCount = InRowCount;
	GridData.ColumnCount = InColumnCount;
	if (!InRowCount || !InColumnCount)
	{
		return GridData;
	}

	FActorSpawnParameters TileGridSpawnParameters;
	TileGridSpawnParameters.Owner = this;
	TileGridSpawnParameters.Instigator = GetInstigator();
	TileGridSpawnParameters.bNoFail = true;
	TileGrid = GetWorld()->SpawnActor<ACppTileGrid>(InCellGridClass, TileGridSpawnParameters);
	TileGrid->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	TileGrid->CellSize = InCellSize;
	TMap<FIntPoint, TObjectPtr<ACppCell>>& GridTiles = TileGrid->Cells;

	for (int32 CoordX = InRowCount / 2 * -1; CoordX < InRowCount / 2; ++CoordX)
	{
		for (int32 CoordY = InColumnCount / 2 * -1; CoordY < InColumnCount / 2; ++CoordY)
		{
			const float CalculatedCoordX = GetActorLocation().X + (CoordY * InCellSize);
			const float CalculatedCoordY = GetActorLocation().Y + (CoordX * InCellSize);
			const FVector TileLocation(CalculatedCoordX, CalculatedCoordY, 0.f);
			FActorSpawnParameters TileSpawnParameters;
			TileSpawnParameters.Owner = TileGrid;
			TileSpawnParameters.Instigator = TileGrid->GetInstigator();
			TileSpawnParameters.bNoFail = true;
			ACppCell* Tile = GetWorld()->SpawnActor<ACppCell>(InCellClass, TileLocation, FRotator::ZeroRotator, TileSpawnParameters);
			Tile->SetActorLabel(FString::Printf(TEXT("Tile_%d_%d"), CoordX, CoordY));
			Tile->AttachToActor(TileGrid, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
			Tile->Coords = FIntPoint(CoordX, CoordY);
			Tile->bIsOccupied = false;
			GridTiles.Add(Tile->Coords, Tile);
			InOnCellSpawnEvent.Execute(Tile);
		}
	}
	return GridData;
}
