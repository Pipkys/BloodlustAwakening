#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Subsystems/WaveSubsystem/CppSpawnSquad.h"

#include "Subsystems/WorldSubsystem.h"
#include "Engine/TimerHandle.h"

#include "CppWaveSubsystem.generated.h"

class ACppSpawner;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWaveStarted, int32, WaveNumber, int32, SquadCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWavesFinished);

UCLASS(BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppWaveSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveSubsystem")
	TArray<FCppSpawnSquad> AvailableSquads;

	UPROPERTY(BlueprintReadOnly, Category = "WaveSubsystem")
	int32 TotalWaves = 0;

	UPROPERTY(BlueprintReadOnly, Category = "WaveSubsystem")
	int32 CurrentWave = 0;

	UPROPERTY(BlueprintReadOnly, Category = "WaveSubsystem")
	float CurrentDifficulty = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveSubsystem", Meta = (ClampMin = "1.0"))
	float WaveInterval = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveSubsystem|Squads")
	int32 BaseSquadCount = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveSubsystem|Squads")
	float SquadGrowthPerWave = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveSubsystem|Squads")
	int32 MaxSquadCount = 20;

	UPROPERTY(BlueprintReadOnly, Category = "WaveSubsystem")
	int32 SquadCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "WaveSubsystem")
	int32 EnemyCount = 0;

	UPROPERTY(BlueprintAssignable, Category = "WaveSubsystem")
	FOnWaveStarted OnWaveStarted;

	UPROPERTY(BlueprintAssignable, Category = "WaveSubsystem")
	FOnWavesFinished OnWavesFinished;

protected:
	virtual void OnWorldComponentsUpdated(UWorld& InWorld) override;

public:
	UFUNCTION(BlueprintCallable, Category = "WaveSubsystem")
	void StartWave(int32 InWaveCount, float InDifficulty);

	UFUNCTION(BlueprintCallable, Category = "WaveSubsystem")
	void EndWave();

	UFUNCTION(BlueprintCallable, Category = "WaveSubsystem")
	void SpawnSquad();

	UFUNCTION(BlueprintPure, Category = "WaveSubsystem")
	int32 CalculateSquadCount(int32 Wave, float Difficulty) const;

	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "WaveSubsystem")
	float GetRemainingTimeToNextWave();

private:
	FTimerHandle WaveTimerHandle;
	bool bWaveActive = false;

	/** Следующая волна: инкремент → расчёт отрядов → спавн → таймер. */
	void BeginNextWave();

	/** Колбэк таймера. */
	void OnWaveTimerElapsed();

	/** Спавн Count отрядов; возвращает число заспавненных монстров. */
	int32 SpawnSquads(int32 Count);

	/** Взвешенный выбор отряда; nullptr — если нет доступных. */
	const FCppSpawnSquad* PickRandomSquad() const;

	ACppSpawner* GetRandomSpawner();
};
