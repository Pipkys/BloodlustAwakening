#include "BloodlustAwakening/Subsystems/CppBuildSubsystem.h"

#include "BloodlustAwakening/Subsystems/CppBuildingRegistry.h"
#include "BloodlustAwakening/Subsystems/CppTeamSubsystem.h"
#include "BloodlustAwakening/GameMode/CppGameplayStatics.h"
#include "BloodlustAwakening/Level/Grid/CppCellGrid.h"

ACppSpawner* UCppBuildSubsystem::BuildSpawner(AActor* InOwner,
	const TArray<ACppCell*>& InOccupiedCells,
	TSubclassOf<ACppSpawner> InClass,
	const FTransform& InOffset)
{
	for (ACppCell* Cell : InOccupiedCells)
	{
		Cell->bIsOccupied = true;
	}

	ACppCellGrid* Grid = InOccupiedCells.Last()->GetGrid();
	ACppCell* LeftCorner = InOccupiedCells[0];
	ACppCell* RightCorner = InOccupiedCells.Last();
	FVector LeftCornerLocation = Grid->GridToWorldP(LeftCorner->Coords);
	FVector RightCornerLocation = Grid->GridToWorldP(RightCorner->Coords);

	FTransform BuildingTransform;
	BuildingTransform.SetLocation(FVector((RightCornerLocation.X - LeftCornerLocation.X) / 2.f + LeftCornerLocation.X,
		(RightCornerLocation.Y - LeftCornerLocation.Y) / 2.f + LeftCornerLocation.Y,
		0.f));

	FCppActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = InOwner;
	SpawnParameters.Instigator = InOwner ? InOwner->GetInstigator() : nullptr;
	ACppSpawner* Spawner = UCppGameplayStatics::SpawnSpawner(InOccupiedCells.Last()->GetWorld(),
		InClass,
		BuildingTransform + InOffset,
		SpawnParameters);
	UCppBuildingRegistry::RegisterBuilding(Spawner, InOccupiedCells);
	return Spawner;
}

ACppDecoration* UCppBuildSubsystem::BuildDecoration(AActor* InOwner,
	const TArray<ACppCell*>& InOccupiedCells,
	TSubclassOf<ACppDecoration> InClass,
	const FTransform& InOffset,
	bool bSmallEnough)
{
	if (!bSmallEnough)
	{
		for (ACppCell* Cell : InOccupiedCells)
		{
			Cell->bIsOccupied = true;
		}
	}

	ACppCellGrid* Grid = InOccupiedCells.Last()->GetGrid();
	ACppCell* LeftCorner = InOccupiedCells[0];
	ACppCell* RightCorner = InOccupiedCells.Last();
	FVector LeftCornerLocation = Grid->GridToWorldP(LeftCorner->Coords);
	FVector RightCornerLocation = Grid->GridToWorldP(RightCorner->Coords);

	FTransform BuildingTransform;
	BuildingTransform.SetLocation(FVector((RightCornerLocation.X - LeftCornerLocation.X) / 2.f + LeftCornerLocation.X,
		(RightCornerLocation.Y - LeftCornerLocation.Y) / 2.f + LeftCornerLocation.Y,
		0.f));

	FCppActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = InOwner;
	SpawnParameters.Instigator = InOwner ? InOwner->GetInstigator() : nullptr;
	ACppDecoration* Decoration = UCppGameplayStatics::SpawnDecoration(InOccupiedCells.Last()->GetWorld(),
		InClass,
		BuildingTransform + InOffset,
		SpawnParameters);
	UCppBuildingRegistry::RegisterBuilding(Decoration, InOccupiedCells);
	return Decoration;
}

ACppTower* UCppBuildSubsystem::BuildTower(AActor* InOwner,
	const TArray<ACppCell*>& InOccupiedCells,
	TSubclassOf<ACppTower> InClass,
	const FTransform& InOffset,
	const FName& InTeamName)
{
	for (ACppCell* Cell : InOccupiedCells)
	{
		Cell->bIsOccupied = true;
	}

	ACppCellGrid* Grid = InOccupiedCells.Last()->GetGrid();
	ACppCell* LeftCorner = InOccupiedCells[0];
	ACppCell* RightCorner = InOccupiedCells.Last();
	FVector LeftCornerLocation = Grid->GridToWorldP(LeftCorner->Coords);
	FVector RightCornerLocation = Grid->GridToWorldP(RightCorner->Coords);

	FTransform BuildingTransform;
	BuildingTransform.SetLocation(FVector((RightCornerLocation.X - LeftCornerLocation.X) / 2.f + LeftCornerLocation.X,
		(RightCornerLocation.Y - LeftCornerLocation.Y) / 2.f + LeftCornerLocation.Y,
		0.f));

	FCppTowerSpawnParameters SpawnParameters;
	SpawnParameters.Owner = InOwner;
	SpawnParameters.Instigator = InOwner ? InOwner->GetInstigator() : nullptr;
	SpawnParameters.Team = InOccupiedCells.Last()->GetWorld()->GetSubsystem<UCppTeamSubsystem>()->GetTeam(InTeamName);
	ACppTower* Tower = UCppGameplayStatics::SpawnTower(InOccupiedCells.Last()->GetWorld(),
		InClass,
		BuildingTransform + InOffset,
		SpawnParameters);
	UCppBuildingRegistry::RegisterBuilding(Tower, InOccupiedCells);
	return Tower;
}
