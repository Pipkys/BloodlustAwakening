#include "BloodlustAwakening/Subsystems/CppBuildSubsystem.h"

#include "BloodlustAwakening/Subsystems/CppBuildingRegistry.h"
#include "BloodlustAwakening/Subsystems/CppTeamSubsystem.h"
#include "BloodlustAwakening/GameMode/CppGameplayStatics.h"

ACppSpawner* UCppBuildSubsystem::BuildSpawner(AActor* InOwner,
	const TArray<ACppCell*>& InOccupiedCells,
	TSubclassOf<ACppSpawner> InClass,
	const FTransform& InTransform)
{
	for (ACppCell* Cell : InOccupiedCells)
	{
		Cell->bIsOccupied = true;
	}
	FCppActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = InOwner;
	SpawnParameters.Instigator = InOwner ? InOwner->GetInstigator() : nullptr;
	ACppSpawner* Spawner = UCppGameplayStatics::SpawnSpawner(InOccupiedCells.Last()->GetWorld(),
		InClass,
		InTransform,
		SpawnParameters);
	UCppBuildingRegistry::RegisterSpawner(Spawner, InOccupiedCells);
	return Spawner;
}

ACppDecoration* UCppBuildSubsystem::BuildDecoration(AActor* InOwner,
	const TArray<ACppCell*>& InOccupiedCells,
	TSubclassOf<ACppDecoration> InClass,
	const FTransform& InTransform,
	bool bSmallEnough)
{
	if (bSmallEnough)
	{
		for (ACppCell* Cell : InOccupiedCells)
		{
			Cell->bIsOccupied = true;
		}
	}
	FCppActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = InOwner;
	SpawnParameters.Instigator = InOwner ? InOwner->GetInstigator() : nullptr;
	ACppDecoration* Decoration = UCppGameplayStatics::SpawnDecoration(InOccupiedCells.Last()->GetWorld(),
		InClass,
		InTransform,
		SpawnParameters);
	UCppBuildingRegistry::RegisterDecoration(Decoration, InOccupiedCells);
	return Decoration;
}

ACppTower* UCppBuildSubsystem::BuildTower(AActor* InOwner,
	const TArray<ACppCell*>& InOccupiedCells,
	TSubclassOf<ACppTower> InClass,
	const FTransform& InTransform,
	const FName& InTeam)
{
	for (ACppCell* Cell : InOccupiedCells)
	{
		Cell->bIsOccupied = true;
	}
	FCppTowerSpawnParameters SpawnParameters;
	SpawnParameters.Owner = InOwner;
	SpawnParameters.Instigator = InOwner ? InOwner->GetInstigator() : nullptr;
	SpawnParameters.Team = InOccupiedCells.Last()->GetWorld()->GetSubsystem<UCppTeamSubsystem>()->GetTeam(InTeam);
	ACppTower* Tower = UCppGameplayStatics::SpawnTower(InOccupiedCells.Last()->GetWorld(),
		InClass,
		InTransform,
		SpawnParameters);
	UCppBuildingRegistry::RegisterBuilding(Tower, InOccupiedCells);
	return Tower;
}
