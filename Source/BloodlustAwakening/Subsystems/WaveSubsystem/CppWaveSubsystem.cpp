#include "BloodlustAwakening/Subsystems/WaveSubsystem/CppWaveSubsystem.h"

#include "BloodlustAwakening/Buildings/Spawners/Implementation/CppSpawner.h"
#include "BloodlustAwakening/Subsystems/CppBuildingRegistry.h"
#include "BloodlustAwakening/GameMode/CppGameplayStatics.h"


#include "Engine/World.h"
#include "TimerManager.h"

void UCppWaveSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	// Страховка: цикл активен, но таймер потерян (пересоздание мира,
	// стриминг) — перезапускаем, чтобы волны не остановились навсегда.
	if (!bWaveActive)
	{
		return;
	}

	FTimerManager& TimerManager = InWorld.GetTimerManager();
	if (!TimerManager.IsTimerActive(WaveTimerHandle))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("UCppWaveSubsystem: wave timer lost, restarting (%.1fs)"), WaveInterval);
		TimerManager.SetTimer(WaveTimerHandle, this,
			&UCppWaveSubsystem::OnWaveTimerElapsed, WaveInterval, false);
	}
}

void UCppWaveSubsystem::Deinitialize()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(WaveTimerHandle);
	}
	bWaveActive = false;
	Super::Deinitialize();
}

void UCppWaveSubsystem::StartWave(int32 InWaveCount, float InDifficulty)
{
	if (InWaveCount <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("UCppWaveSubsystem::StartWave: wave count must be > 0"));
		return;
	}

	if (!AvailableSquads.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("UCppWaveSubsystem::StartWave: no available squads"));
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Перезапуск поверх уже идущего цикла.
	World->GetTimerManager().ClearTimer(WaveTimerHandle);

	TotalWaves = InWaveCount;
	CurrentDifficulty = FMath::Max(InDifficulty, 0.f);
	CurrentWave = 0;
	SquadCount = 0;
	EnemyCount = 0;
	bWaveActive = true;

	UE_LOG(LogTemp, Display,
		TEXT("UCppWaveSubsystem: start %d wave(s), difficulty %.2f, interval %.1fs"),
		TotalWaves, CurrentDifficulty, WaveInterval);

	// Первая волна — сразу.
	BeginNextWave();
}

void UCppWaveSubsystem::EndWave()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(WaveTimerHandle);
	}
	bWaveActive = false;
	TotalWaves = 0;
	CurrentWave = 0;
	SquadCount = 0;
	EnemyCount = 0;
	UE_LOG(LogTemp, Display, TEXT("UCppWaveSubsystem: wave cycle stopped"));
}

void UCppWaveSubsystem::BeginNextWave()
{
	if (!bWaveActive)
	{
		return;
	}
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Последняя волна отработала свой интервал — завершаем цикл.
	if (CurrentWave >= TotalWaves)
	{
		bWaveActive = false;
		World->GetTimerManager().ClearTimer(WaveTimerHandle);
		UE_LOG(LogTemp, Display,
			TEXT("UCppWaveSubsystem: all %d wave(s) finished"), TotalWaves);
		OnWavesFinished.Broadcast();
		return;
	}

	++CurrentWave;
	SquadCount = CalculateSquadCount(CurrentWave, CurrentDifficulty);
	EnemyCount = 0;

	UE_LOG(LogTemp, Display,
		TEXT("UCppWaveSubsystem: wave %d/%d — %d squad(s)"),
		CurrentWave, TotalWaves, SquadCount);

	EnemyCount = SpawnSquads(SquadCount);

	OnWaveStarted.Broadcast(CurrentWave, SquadCount);

	// Таймер до следующей волны.
	World->GetTimerManager().SetTimer(WaveTimerHandle, this,
		&UCppWaveSubsystem::OnWaveTimerElapsed, WaveInterval, false);
}

void UCppWaveSubsystem::OnWaveTimerElapsed()
{
	BeginNextWave();
}

int32 UCppWaveSubsystem::CalculateSquadCount(int32 Wave, float Difficulty) const
{
	// По мотивам Killing Floor: базовое число отрядов растёт с номером волны
	// и умножается на множитель сложности, с потолком MaxSquadCount.
	const float DifficultyScale = FMath::Max(Difficulty, 0.1f);
	const float Raw = (BaseSquadCount + SquadGrowthPerWave * (Wave - 1)) * DifficultyScale;
	return FMath::Clamp(FMath::RoundToInt(Raw), 1, MaxSquadCount);
}

int32 UCppWaveSubsystem::SpawnSquads(int32 Count)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return 0;
	}

	int32 SpawnedMonsters = 0;
	for (int32 Index = 0; Index < Count; ++Index)
	{
		const FCppSpawnSquad* Squad = PickRandomSquad();
		if (!Squad)
		{
			UE_LOG(LogTemp, Warning,
				TEXT("UCppWaveSubsystem::SpawnSquads: no valid squads (wave %d)"), CurrentWave);
			break;
		}

		ACppSpawner* Spawner = GetRandomSpawner();
		if (!Spawner)
		{
			UE_LOG(LogTemp, Warning,
				TEXT("UCppWaveSubsystem::SpawnSquads: no spawners registered"));
			break;
		}

		FCppUnitSpawnParameters SpawnParameters;
		for (const TSubclassOf<ACppUnit>& UnitClass : Squad->Units)
		{
			if (UnitClass && UCppGameplayStatics::SpawnUnit(this, UnitClass, Spawner->GetTransform(), SpawnParameters))
			{
				++SpawnedMonsters;
			}
		}
	}
	return SpawnedMonsters;
}

void UCppWaveSubsystem::SpawnSquad()
{
	SpawnSquads(1);
}

const FCppSpawnSquad* UCppWaveSubsystem::PickRandomSquad() const
{
	TArray<const FCppSpawnSquad*> Valid;
	TArray<float> Weights;
	float TotalWeight = 0.f;

	for (const FCppSpawnSquad& Squad : AvailableSquads)
	{
		const float Weight = Squad.GetWeight(CurrentWave, CurrentDifficulty);
		if (Weight > 0.f)
		{
			Valid.Add(&Squad);
			Weights.Add(Weight);
			TotalWeight += Weight;
		}
	}

	if (Valid.IsEmpty())
	{
		return nullptr;
	}

	float RandomValue = FMath::FRandRange(0.f, TotalWeight);
	for (int32 Index = 0; Index < Weights.Num(); ++Index)
	{
		RandomValue -= Weights[Index];
		if (RandomValue <= 0.f)
		{
			return Valid[Index];
		}
	}
	return Valid.Last();
}

ACppSpawner* UCppWaveSubsystem::GetRandomSpawner()
{
	TArray<TScriptInterface<ICppGridPlaceable>>& Spawners =
		UCppBuildingRegistry::GetAllBuildingsOfClass(this, ACppSpawner::StaticClass());
	if (Spawners.Num() == 0)
	{
		return nullptr;
	}

	const int32 Index = FMath::RandRange(0, Spawners.Num() - 1);
	return Cast<ACppSpawner>(Spawners[Index].GetObject());
}
