#include "BloodlustAwakening/Subsystems/CppGridSubsystem.h"

void UCppGridSubsystem::SetupGenerator(ECppGridType InGridType,
	TSubclassOf<ACppCellGridGenerator> InGeneratorClass,
	TSubclassOf<ACppCellGrid> InGridClass,
	TSubclassOf<ACppCell> InCellClass,
	float InCellSize)
{
	FCppGeneratorData GeneratorData;
	GeneratorData.Generator = GetWorld()->SpawnActor<ACppCellGridGenerator>(InGeneratorClass);
	GeneratorData.GridClass = InGridClass;
	GeneratorData.CellClass = InCellClass;
	GeneratorData.CellSize = InCellSize;
	if (FCppGeneratorData* FoundGeneratorData = GeneratorsData.Find(InGridType))
	{
		UE_LOG(LogTemp, Warning, TEXT("UCppGridSubsystem::SetupGenerator: Changing existing generator setup"));
		FoundGeneratorData->Generator->Destroy();
		*FoundGeneratorData = std::move(GeneratorData);
		return;
	}
	GeneratorsData.Emplace(std::move(InGridType), std::move(GeneratorData));
}

FCppCircleGridData UCppGridSubsystem::GenerateCircleGrid(ECppGridType InGridType,
	int32 InRadius,
	const FCppOnCellSpawnedEvent& InOnCellSpawnEvent)
{
	if (FCppGeneratorData* FoundGeneratorData = GeneratorsData.Find(InGridType))
	{
		return FoundGeneratorData->Generator->GenerateCircleGrid(FoundGeneratorData->CellClass, 
			FoundGeneratorData->GridClass, 
			FoundGeneratorData->CellSize,
			InRadius,
			InOnCellSpawnEvent);
	}
	UE_LOG(LogTemp, Fatal, TEXT("UCppGridSubsystem::GenerateCircleGrid: Attempting to generate circle grid, without generator setup"));
	return FCppCircleGridData();
}

FCppRectangleGridData UCppGridSubsystem::GenerateRectangleGrid(ECppGridType InGridType,
	int32 InRowCount,
	int32 InColumnCount,
	const FCppOnCellSpawnedEvent& InOnCellSpawnEvent)
{
	if (FCppGeneratorData* FoundGeneratorData = GeneratorsData.Find(InGridType))
	{
		return FoundGeneratorData->Generator->GenerateRectangleGrid(FoundGeneratorData->CellClass, 
			FoundGeneratorData->GridClass,
			FoundGeneratorData->CellSize, 
			InRowCount,
			InColumnCount,
			InOnCellSpawnEvent);
	}
	UE_LOG(LogTemp, Fatal, TEXT("UCppGridSubsystem::GenerateRectangleGrid: Attempting to generate rectangle grid, without generator setup"));
	return FCppRectangleGridData();
}
