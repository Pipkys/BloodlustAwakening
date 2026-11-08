#pragma once

#include "CoreMinimal.h"

#include "Subsystems/WorldSubsystem.h"
#include "BloodlustAwakening/Subsystems/WaveSubsystem/CppSpawnSquad.h"

#include "CppWaveSubsystem.generated.h"

class ACppSpawner;

UCLASS(BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppWaveSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveSubsystem")
	TArray<FCppSpawnSquad> AvailableSquads;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveSubsystem")
	int32 CurrentWave = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveSubsystem")
	float CurrentDifficulty = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveSubsystem")
	int32 EnemyCount = 0;

protected:
	void OnWorldComponentsUpdated(UWorld& InWorld) override;

public:
	UFUNCTION(BlueprintCallable, Category = "WaveSubsystem")
	void StartWave(int32 InWaveNumber, float InDifficulty);

	UFUNCTION(BlueprintCallable, Category = "WaveSubsystem")
	void EndWave();

	UFUNCTION(BlueprintCallable, Category = "WaveSubsystem")
	void SpawnSquad();

private:
	ACppSpawner* GetRandomSpawner();
};
