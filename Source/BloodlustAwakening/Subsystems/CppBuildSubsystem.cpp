#include "BloodlustAwakening/Subsystems/CppBuildSubsystem.h"

#include "BloodlustAwakening/Subsystems/CppBuildingRegistry.h"
#include "BloodlustAwakening/Subsystems/CppTeamSubsystem.h"
#include "BloodlustAwakening/GameMode/CppGameplayStatics.h"
#include "BloodlustAwakening/Buildings/Towers/Implementation/CppTowerSpawnParameters.h"
#include "BloodlustAwakening/Level/Grid/CppCellGrid.h"

namespace
{
	// Центр многоугольника, образованного занятыми ячейками (середина
	// отрезка между крайними ячейками) — одинаков для всех типов построек.
	FTransform ComputeBuildingTransform(const TArray<ACppCell*>& InOccupiedCells)
	{
		ACppCell* LeftCorner = InOccupiedCells[0];
		ACppCell* RightCorner = InOccupiedCells.Last();
		ACppCellGrid* Grid = RightCorner->GetGrid();
		const FVector LeftCornerLocation = Grid->GridToWorldP(LeftCorner->Coords);
		const FVector RightCornerLocation = Grid->GridToWorldP(RightCorner->Coords);

		FTransform BuildingTransform;
		BuildingTransform.SetLocation(FVector(
			(RightCornerLocation.X - LeftCornerLocation.X) / 2.f + LeftCornerLocation.X,
			(RightCornerLocation.Y - LeftCornerLocation.Y) / 2.f + LeftCornerLocation.Y,
			0.f));
		return BuildingTransform;
	}

	bool ValidateCells(const TCHAR* InContext, const TArray<ACppCell*>& InOccupiedCells)
	{
		if (InOccupiedCells.IsEmpty())
		{
			UE_LOG(LogTemp, Error, TEXT("UCppBuildSubsystem::%s: empty cells array"), InContext);
			return false;
		}
		for (ACppCell* Cell : InOccupiedCells)
		{
			if (!IsValid(Cell))
			{
				UE_LOG(LogTemp, Error, TEXT("UCppBuildSubsystem::%s: invalid cell in array"), InContext);
				return false;
			}
		}
		return true;
	}

	void SetCellsOccupied(const TArray<ACppCell*>& InOccupiedCells, bool bOccupied)
	{
		for (ACppCell* Cell : InOccupiedCells)
		{
			Cell->bIsOccupied = bOccupied;
		}
	}
}

ACppSpawner* UCppBuildSubsystem::BuildSpawner(AActor* InOwner,
	const TArray<ACppCell*>& InOccupiedCells,
	TSubclassOf<ACppSpawner> InClass,
	const FTransform& InOffset)
{
	if (!ValidateCells(TEXT("BuildSpawner"), InOccupiedCells) || !InClass)
	{
		return nullptr;
	}

	const FTransform BuildingTransform = ComputeBuildingTransform(InOccupiedCells);

	FCppActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = InOwner;
	SpawnParameters.Instigator = InOwner ? InOwner->GetInstigator() : nullptr;
	ACppSpawner* Spawner = UCppGameplayStatics::SpawnSpawner(InOccupiedCells.Last()->GetWorld(),
		InClass,
		BuildingTransform + InOffset,
		SpawnParameters);
	SetCellsOccupied(InOccupiedCells, true);
	UCppBuildingRegistry::RegisterBuilding(Spawner, InOccupiedCells);
	return Spawner;
}

ACppDecoration* UCppBuildSubsystem::BuildDecoration(AActor* InOwner,
	const TArray<ACppCell*>& InOccupiedCells,
	TSubclassOf<ACppDecoration> InClass,
	const FTransform& InOffset,
	bool bSmallEnough)
{
	if (!ValidateCells(TEXT("BuildDecoration"), InOccupiedCells) || !InClass)
	{
		return nullptr;
	}

	const FTransform BuildingTransform = ComputeBuildingTransform(InOccupiedCells);

	FCppActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = InOwner;
	SpawnParameters.Instigator = InOwner ? InOwner->GetInstigator() : nullptr;
	ACppDecoration* Decoration = UCppGameplayStatics::SpawnDecoration(InOccupiedCells.Last()->GetWorld(),
		InClass,
		BuildingTransform + InOffset,
		SpawnParameters);
	if (!bSmallEnough)
	{
		SetCellsOccupied(InOccupiedCells, true);
	}
	UCppBuildingRegistry::RegisterBuilding(Decoration, InOccupiedCells);
	return Decoration;
}

ACppTower* UCppBuildSubsystem::BuildTower(AActor* InOwner,
	const TArray<ACppCell*>& InOccupiedCells,
	TSubclassOf<ACppTower> InClass,
	const FTransform& InOffset,
	const FName& InTeamName)
{
	if (!ValidateCells(TEXT("BuildTower"), InOccupiedCells) || !InClass)
	{
		return nullptr;
	}

	const FTransform BuildingTransform = ComputeBuildingTransform(InOccupiedCells);

	FCppTowerSpawnParameters SpawnParameters;
	SpawnParameters.Owner = InOwner;
	SpawnParameters.Instigator = InOwner ? InOwner->GetInstigator() : nullptr;
	UCppTeamSubsystem* TeamSubsystem = InOccupiedCells.Last()->GetWorld()->GetSubsystem<UCppTeamSubsystem>();
	SpawnParameters.Team = TeamSubsystem ? TeamSubsystem->GetTeam(InTeamName) : nullptr;
	ACppTower* Tower = UCppGameplayStatics::SpawnTower(InOccupiedCells.Last()->GetWorld(),
		InClass,
		BuildingTransform + InOffset,
		SpawnParameters);
	if (!Tower)
	{
		return nullptr;
	}

	SetCellsOccupied(InOccupiedCells, true);
	UCppBuildingRegistry::RegisterBuilding(Tower, InOccupiedCells);
	return Tower;
}
