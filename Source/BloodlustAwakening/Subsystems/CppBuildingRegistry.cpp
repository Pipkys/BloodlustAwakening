#include "BloodlustAwakening/Subsystems/CppBuildingRegistry.h"

void UCppBuildingRegistry::RegisterBuilding(TScriptInterface<ICppGridPlaceable> InBuilding, const TArray<ACppCell*>& InOccupiedCells)
{
	if (Buildings.FindByPredicate([InBuilding](TScriptInterface<ICppGridPlaceable>& Building)
	{
		return Building == InBuilding;
	}))
	{
		UE_LOG(LogTemp, Error, TEXT("UCppBuildingRegistry::RegisterBuilding: Attempting to register already registred building"));
		return;
	}
	InBuilding->GetOccupiedCells() = InOccupiedCells;
	for (ACppCell* Cell : InBuilding->GetOccupiedCells())
	{
		Cell->bIsOccupied = true;
	}
	Buildings.Add(InBuilding);
}

void UCppBuildingRegistry::UnregisterBuilding(TScriptInterface<ICppGridPlaceable> InBuilding)
{
	for (int32 Index = 0; Index < Buildings.Num(); ++Index)
	{
		if (Buildings[Index] == InBuilding)
		{
			Buildings.RemoveAt(Index);
			for (ACppCell* Cell : InBuilding->GetOccupiedCells())
			{
				Cell->bIsOccupied = false;
			}
			return;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("UCppBuildingRegistry::UnregisterBuilding: Attempting to unregister not registred building"));
}

void UCppBuildingRegistry::RegisterDecoration(TScriptInterface<ICppGridPlaceable> InDecoration, const TArray<ACppCell*>& InOccupiedCells, bool bSmallEnough)
{
	if (Decorations.FindByPredicate([InDecoration](TScriptInterface<ICppGridPlaceable>& Decoration)
	{
		return Decoration == InDecoration;
	}))
	{
		UE_LOG(LogTemp, Error, TEXT("UCppBuildingRegistry::RegisterDecoration: Attempting to register already registred decoration"));
		return;
	}
	InDecoration->GetOccupiedCells() = InOccupiedCells;
	if (!bSmallEnough)
	{
		for (ACppCell* Cell : InDecoration->GetOccupiedCells())
		{
			Cell->bIsOccupied = true;
		}
	}
	Decorations.Add(InDecoration);
}

void UCppBuildingRegistry::UnregisterDecoration(TScriptInterface<ICppGridPlaceable> InDecoration)
{
	for (int32 Index = 0; Index < Decorations.Num(); ++Index)
	{
		if (Decorations[Index] == InDecoration)
		{
			Decorations.RemoveAt(Index);
			for (ACppCell* Cell : InDecoration->GetOccupiedCells())
			{
				Cell->bIsOccupied = false;
			}
			return;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("UCppBuildingRegistry::UnregisterDecoration: Attempting to unregister not registred decoration"));
}

void UCppBuildingRegistry::RegisterSpawner(TScriptInterface<ICppGridPlaceable> InSpawner, const TArray<ACppCell*>& InOccupiedCells)
{
	if (Spawners.FindByPredicate([InSpawner](TScriptInterface<ICppGridPlaceable>& Spawner)
	{
		return Spawner == InSpawner;
	}))
	{
		UE_LOG(LogTemp, Error, TEXT("UCppBuildingRegistry::RegisterSpawner: Attempting to register already registred spawner"));
		return;
	}
	InSpawner->GetOccupiedCells() = InOccupiedCells;
	for (ACppCell* Cell : InSpawner->GetOccupiedCells())
	{
		Cell->bIsOccupied = true;
	}
	Spawners.Add(InSpawner);
}

void UCppBuildingRegistry::UnregisterSpawner(TScriptInterface<ICppGridPlaceable> InSpawner)
{
	for (int32 Index = 0; Index < Spawners.Num(); ++Index)
	{
		if (Spawners[Index] == InSpawner)
		{
			Spawners.RemoveAt(Index);
			for (ACppCell* Cell : InSpawner->GetOccupiedCells())
			{
				Cell->bIsOccupied = false;
			}
			return;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("UCppBuildingRegistry::UnregisterSpawner: Attempting to unregister not registred spawner"));
}

TScriptInterface<ICppGridPlaceable> UCppBuildingRegistry::GetBuildingByOccupiedCells(const TArray<ACppCell*>& InOccupiedCells)
{
	if (TScriptInterface<ICppGridPlaceable>* Building
		= Buildings.FindByPredicate([InOccupiedCells](TScriptInterface<ICppGridPlaceable>& Building)
	{
		if (Building->GetOccupiedCells().Num() != InOccupiedCells.Num())
		{
			return false;
		}
		for (int32 Index = 0; Index < Building->GetOccupiedCells().Num(); ++Index)
		{
			if (Building->GetOccupiedCells()[Index] != InOccupiedCells[Index])
			{
				return false;
			}
		}
		return true;
	}))
	{
		return *Building;
	}
	UE_LOG(LogTemp, Error, TEXT("UCppBuildingRegistry::GetOccupiedCellsByBuilding: Attempting to get building by occupied cells which not stored"));
	return nullptr;
}

TScriptInterface<ICppGridPlaceable> UCppBuildingRegistry::GetBuildingByOccupiedCellCoords(const TArray<FIntPoint>& InOccupiedCells)
{
	if (TScriptInterface<ICppGridPlaceable>* Building
		= Buildings.FindByPredicate([InOccupiedCells](TScriptInterface<ICppGridPlaceable>& Building)
	{
		if (Building->GetOccupiedCells().Num() != InOccupiedCells.Num())
		{
			return false;
		}
		for (int32 Index = 0; Index < Building->GetOccupiedCells().Num(); ++Index)
		{
			if (Building->GetOccupiedCells()[Index]->Coords != InOccupiedCells[Index])
			{
				return false;
			}
		}
		return true;
	}))
	{
		return *Building;
	}
	UE_LOG(LogTemp, Error, TEXT("UCppBuildingRegistry::GetOccupiedCellsByBuilding: Attempting to get building by occupied cells which not stored"));
	return nullptr;
}
