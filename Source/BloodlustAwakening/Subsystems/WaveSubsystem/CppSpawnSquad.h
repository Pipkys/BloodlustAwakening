#pragma once

#include "CoreMinimal.h"

#include "CppSpawnSquad.generated.h"

USTRUCT(BlueprintType)
struct FCppSpawnSquad
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AActor>> Units;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseWeight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinWave = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxWave = 999;

public:
	float GetWeight(int32 InWaveNumber, float InDifficulty) const;
};