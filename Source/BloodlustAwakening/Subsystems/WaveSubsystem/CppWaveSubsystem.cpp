#include "BloodlustAwakening/Subsystems/WaveSubsystem/CppWaveSubsystem.h"

#include "BloodlustAwakening/Buildings/Spawners/Implementation/CppSpawner.h"
#include "BloodlustAwakening/Subsystems/CppBuildingRegistry.h"

#include "Kismet/GameplayStatics.h"

void UCppWaveSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	//if (CurrentDifficulty <= 0.f)
	//{
	//	return;
	//}
	//
	//if (EnemyCount == 0)
	//{
	//	EndWave();
	//	return;
	//}
}

void UCppWaveSubsystem::StartWave(int32 WaveNumber, float Difficulty)
{
	CurrentWave = WaveNumber;
	CurrentDifficulty = Difficulty;
	//EnemyCount = (float)CurrentWave * Difficulty;
}

void UCppWaveSubsystem::EndWave()
{
	CurrentWave = 0;
	CurrentDifficulty = 0.f;
}

void UCppWaveSubsystem::SpawnSquad()
{
	ACppSpawner* Spawner = GetRandomSpawner();
	if (!Spawner)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCppWaveSubsystem::SpawnSquad: no spawners available"));
		return;
	}

	TArray<FCppSpawnSquad> ValidSquads;
	TArray<float> Weights;
	float TotalWeight = 0.f;

	for (const FCppSpawnSquad& Squad : AvailableSquads)
	{
		float Weight = Squad.GetWeight(CurrentWave, CurrentDifficulty);
		if (Weight > 0.0f)
		{
			ValidSquads.Add(Squad);
			Weights.Add(Weight);
			TotalWeight += Weight;
		}
	}

	if (!ValidSquads.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("UCppWaveSubsystem::SpawnSquad: no squads available"));
		return;
	}

	float RandomValue = FMath::FRandRange(0.f, TotalWeight);
	int32 SelectedIndex = 0;
	float Accumulated = 0.f;

	for (int32 i = 0; i < Weights.Num(); ++i)
	{
		Accumulated += Weights[i];
		if (RandomValue <= Accumulated)
		{
			SelectedIndex = i;
			break;
		}
	}

	const FCppSpawnSquad& SelectedSquad = ValidSquads[SelectedIndex];
	const FVector SpawnLocation = Spawner->GetActorLocation();
	const FRotator SpawnRotation = Spawner->GetActorRotation();

	for (TSubclassOf<AActor> UnitClass : SelectedSquad.Units)
	{
		if (UnitClass)
		{
			GetWorld()->SpawnActor<AActor>(UnitClass, SpawnLocation, SpawnRotation);
		}
	}
}

ACppSpawner* UCppWaveSubsystem::GetRandomSpawner()
{
	TArray<TScriptInterface<ICppGridPlaceable>>& Spawners = UCppBuildingRegistry::GetAllBuildingsOfClass(this, ACppSpawner::StaticClass());
	if (Spawners.Num() == 0)
	{
		return nullptr;
	}

	const int32 Index = FMath::RandRange(0, Spawners.Num() - 1);
	return Cast<ACppSpawner>(Spawners[Index].GetObject());
}