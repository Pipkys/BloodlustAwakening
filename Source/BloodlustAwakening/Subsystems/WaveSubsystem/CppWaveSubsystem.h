#pragma once

#include "CoreMinimal.h"

#include "Subsystems/WorldSubsystem.h"
#include "Engine/TimerHandle.h"
#include "BloodlustAwakening/Subsystems/WaveSubsystem/CppSpawnSquad.h"

#include "CppWaveSubsystem.generated.h"

class ACppSpawner;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWaveStarted, int32, WaveNumber, int32, SquadCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWavesFinished);

UCLASS(BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppWaveSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/** Отряды врагов — задаются до вызова StartWave (в C++ или BP). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveSubsystem")
	TArray<FCppSpawnSquad> AvailableSquads;

	// --- Параметры цикла (задаются в StartWave) ---
	UPROPERTY(BlueprintReadOnly, Category = "WaveSubsystem")
	int32 TotalWaves = 0;

	UPROPERTY(BlueprintReadOnly, Category = "WaveSubsystem")
	int32 CurrentWave = 0;

	UPROPERTY(BlueprintReadOnly, Category = "WaveSubsystem")
	float CurrentDifficulty = 1.0f;

	/** Пауза между волнами, сек. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveSubsystem", meta = (ClampMin = "1.0"))
	float WaveInterval = 30.f;

	// --- KF-стиль: расчёт количества отрядов на волну ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveSubsystem|Squads")
	int32 BaseSquadCount = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveSubsystem|Squads")
	float SquadGrowthPerWave = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveSubsystem|Squads")
	int32 MaxSquadCount = 20;

	/** Отрядов заспавнено в текущей волне. */
	UPROPERTY(BlueprintReadOnly, Category = "WaveSubsystem")
	int32 SquadCount = 0;

	/** Монстров заспавнено в текущей волне. */
	UPROPERTY(BlueprintReadOnly, Category = "WaveSubsystem")
	int32 EnemyCount = 0;

	// --- События для BP (UI/логика игры) ---
	UPROPERTY(BlueprintAssignable, Category = "WaveSubsystem")
	FOnWaveStarted OnWaveStarted;

	UPROPERTY(BlueprintAssignable, Category = "WaveSubsystem")
	FOnWavesFinished OnWavesFinished;

protected:
	/**
	 * Страховка цикла: если волны идут, а таймер потерян — перезапускаем.
	 * (В UE5.8 нет OnWorldComponentsBeginUpdate, есть только этот колбэк.)
	 */
	virtual void OnWorldComponentsUpdated(UWorld& InWorld) override;

public:
	/**
	 * Точка входа: StartWave(количество волн, сложность).
	 * Первая волна стартует немедленно, далее каждые WaveInterval секунд
	 * запускается следующая, пока не завершится последняя волна.
	 * Отряды задаются через AvailableSquads ДО вызова.
	 */
	UFUNCTION(BlueprintCallable, Category = "WaveSubsystem")
	void StartWave(int32 InWaveCount, float InDifficulty);

	/** Принудительно остановить цикл волн и сбросить состояние. */
	UFUNCTION(BlueprintCallable, Category = "WaveSubsystem")
	void EndWave();

	/** Спавн одного отряда (взвешенный случайный выбор). */
	UFUNCTION(BlueprintCallable, Category = "WaveSubsystem")
	void SpawnSquad();

	/** KF-формула: сколько отрядов на волне Wave при сложности Difficulty. */
	UFUNCTION(BlueprintPure, Category = "WaveSubsystem")
	int32 CalculateSquadCount(int32 Wave, float Difficulty) const;

	virtual void Deinitialize() override;

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
