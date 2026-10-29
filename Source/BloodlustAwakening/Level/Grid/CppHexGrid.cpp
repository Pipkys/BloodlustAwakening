#include "BloodlustAwakening/Level/Grid/CppHexGrid.h"

FIntPoint ACppHexGrid::WorldToGrid(const FVector& InWorldLocation) const
{
	const float ClampedHexSize = FMath::Max(10.0f, CellSize);
	FVector LocalPos = InWorldLocation - GetActorLocation();

	float CoordQFloat = LocalPos.X / (ClampedHexSize * 1.5f);
	float CoordRFloat = (LocalPos.Y / (ClampedHexSize * FMath::Sqrt(3.f))) - (0.5f * CoordQFloat);
	float CoordSFloat = -CoordQFloat - CoordRFloat;

	int CoordQInt = FMath::RoundToInt(CoordQFloat);
	int CoordRInt = FMath::RoundToInt(CoordRFloat);
	int CoordSInt = FMath::RoundToInt(CoordSFloat);

	int MaxDist = FMath::Abs(CoordQInt - CoordQFloat);
	int DistR = FMath::Abs(CoordRInt - CoordRFloat);
	int DistS = FMath::Abs(CoordSInt - CoordSFloat);

	if (DistR > MaxDist)
	{
		MaxDist = DistR;
	}

	if (DistS > MaxDist)
	{
		MaxDist = DistS;
	}

	if (MaxDist == FMath::Abs(CoordQInt - CoordQFloat))
	{
		CoordQInt = -CoordRInt - CoordSInt;
	}
	else if (MaxDist == FMath::Abs(CoordRInt - CoordRFloat))
	{
		CoordRInt = -CoordQInt - CoordSInt;
	}
	else
	{
		CoordSInt = -CoordQInt - CoordRInt;
	}

	return FIntPoint(CoordQInt, CoordRInt);
}

FVector ACppHexGrid::GridToWorld(int32 InCoordQ, int32 InCoordR) const
{
	FVector Location = CalculateHexLocation(InCoordQ, InCoordR);
	return Location + GetActorLocation();
}

FVector ACppHexGrid::CalculateHexLocation(int32 InCoordQ, int32 InCoordR) const
{
	const float ClampedHexSize = FMath::Max(10.0f, CellSize);
	float PosX = ClampedHexSize * 1.5f * InCoordQ;
	float PosY = ClampedHexSize * FMath::Sqrt(3.f) * (InCoordR + 0.5f * InCoordQ);
	return FVector(PosX, PosY, 0.0f);
}
