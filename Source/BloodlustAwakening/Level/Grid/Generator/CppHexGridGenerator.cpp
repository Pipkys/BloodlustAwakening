#include "BloodlustAwakening/Level/Grid/Generator/CppHexGridGenerator.h"

#include "BloodlustAwakening/Level/Grid/CppHexGrid.h"

#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

FCppCircleGridData ACppHexGridGenerator::GenerateCircleGrid(TSubclassOf<ACppCell> InCellClass,
	TSubclassOf<ACppCellGrid> InCellGridClass,
	float InCellSize,
	int32 InRadius,
	const FCppOnCellSpawnedEvent& InOnCellSpawnEvent)
{
	FCppCircleGridData GridData;
	TObjectPtr<ACppCellGrid>& HexGrid = GridData.CellGrid;
	HexGrid = nullptr;
	GridData.Radius = InRadius;

	if (InRadius < 1)
	{
		return GridData;
	}

	FActorSpawnParameters HexGridSpawnParameters;
	HexGridSpawnParameters.Owner = this;
	HexGridSpawnParameters.Instigator = GetInstigator();
	HexGridSpawnParameters.bNoFail = true;
	HexGrid = GetWorld()->SpawnActor<ACppHexGrid>(InCellGridClass, HexGridSpawnParameters);
	HexGrid->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	HexGrid->CellSize = InCellSize;
	TMap<FIntPoint, TObjectPtr<ACppCell>>& GridHexes = HexGrid->Cells;

	for (int32 CoordQ = -InRadius; CoordQ <= InRadius; ++CoordQ)
	{
		for (int32 CoordR = -InRadius; CoordR <= InRadius; ++CoordR)
		{
			if (!IsHexInRadius(InCellSize, InRadius, CoordQ, CoordR))
			{
				continue;
			}

			FVector HexLocation = CalculateHexLocation(InCellSize, CoordQ, CoordR);
			FActorSpawnParameters HexSpawnParameters;
			HexSpawnParameters.Owner = HexGrid;
			HexSpawnParameters.Instigator = HexGrid->GetInstigator();
			HexSpawnParameters.bNoFail = true;
			ACppCell* Hex = GetWorld()->SpawnActor<ACppCell>(InCellClass, HexLocation, FRotator::ZeroRotator, HexSpawnParameters);
			Hex->SetActorLabel(FString::Printf(TEXT("Hex_%d_%d"), CoordQ, CoordR));
			Hex->AttachToActor(HexGrid, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
			Hex->Coords = FIntPoint(CoordQ, CoordR);
			Hex->bIsOccupied = false;
			GridHexes.Add(Hex->Coords, Hex);
			InOnCellSpawnEvent.Execute(Hex);
		}
	}
	return GridData;
}

FCppRectangleGridData ACppHexGridGenerator::GenerateRectangleGrid(
    TSubclassOf<ACppCell> InCellClass,
    TSubclassOf<ACppCellGrid> InCellGridClass,
    float InCellSize,
    int32 InRowCount,
    int32 InColumnCount,
    const FCppOnCellSpawnedEvent& InOnCellSpawnEvent)
{
	//FCppRectangleGridData GridData;
	//TObjectPtr<ACppCellGrid>& HexGrid = GridData.CellGrid;
	//HexGrid = nullptr;
	//GridData.RowCount = InRowCount;
	//GridData.ColumnCount = InColumnCount;
	//
	//if (!InRowCount || !InColumnCount)
	//{
	//    return GridData;
	//}
	//
	//FActorSpawnParameters HexGridSpawnParameters;
	//HexGridSpawnParameters.Owner = this;
	//HexGridSpawnParameters.Instigator = GetInstigator();
	//HexGridSpawnParameters.bNoFail = true;
	//HexGrid = GetWorld()->SpawnActor<ACppHexGrid>(InCellGridClass, HexGridSpawnParameters);
	//HexGrid->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//HexGrid->CellSize = InCellSize;
	//TArray<FCppCellData>& GridHexes = HexGrid->Data;
	//
	//const float CellHeight = InCellSize * FMath::Sqrt(3.0f);
	//const float HorizontalStep = InCellSize * 0.75f;
	//const float VerticalStep = CellHeight;
	//
	//const FVector BaseLocation = GetActorLocation();
	//
	//for (int32 Col = 0; Col < InColumnCount; ++Col)
	//{
	//    for (int32 Row = 0; Row < InRowCount; ++Row)
	//    {
	//        float WorldX = BaseLocation.X + (Col * HorizontalStep);
	//        float WorldY = BaseLocation.Y + (Row * VerticalStep);
	//
	//        if (Col % 2 != 0)
	//        {
	//            WorldY += VerticalStep * 0.5f;
	//        }
	//
	//        WorldX += InCellSize * 1.5f;
	//
	//        FVector HexLocation(WorldX, WorldY, 0.f);
	//
	//        FActorSpawnParameters HexSpawnParameters;
	//        HexSpawnParameters.Owner = HexGrid;
	//        HexSpawnParameters.Instigator = HexGrid->GetInstigator();
	//        HexSpawnParameters.bNoFail = true;
	//        ACppHex* Hex = GetWorld()->SpawnActor<ACppHex>(InCellClass, HexLocation, FRotator::ZeroRotator, HexSpawnParameters);
	//        Hex->SetActorLabel(FString::Printf(TEXT("Hex_%d_%d"), Row, Col));
	//        Hex->AttachToActor(HexGrid, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//
	//        FCppCellData HexData;
	//        HexData.Cell = Hex;
	//        HexData.Coords = FIntPoint(Row, Col);
	//        HexData.bIsOccupied = false;
	//        GridHexes.Add(HexData);
	//        InOnCellSpawnEvent.Execute(HexData);
	//    }
	//}
	//
	//return GridData;
	return {};
}

FVector ACppHexGridGenerator::CalculateHexLocation(float InCellSize, int32 InCoordQ, int32 InCoordR) const
{
	const float ClampedHexSize = FMath::Max(10.0f, InCellSize);
	float PosX = ClampedHexSize * 1.5f * InCoordQ;
	float PosY = ClampedHexSize * FMath::Sqrt(3.f) * (InCoordR + 0.5f * InCoordQ);
	return FVector(PosX, PosY, 0.0f);
}

bool ACppHexGridGenerator::IsHexInRadius(float InCellSize, int32 Radius, int32 InCoordQ, int32 InCoordR) const
{
	int CoordS = -InCoordQ - InCoordR;
	int Dist = (FMath::Abs(InCoordQ) + FMath::Abs(InCoordR) + FMath::Abs(CoordS)) / 2;
	return Dist <= Radius;
}
