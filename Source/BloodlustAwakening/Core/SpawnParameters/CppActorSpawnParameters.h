#pragma once

#include "CoreMinimal.h"

#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"

#include "CppActorSpawnParameters.generated.h"

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppActorSpawnParameters
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Parameters")
	AActor* Owner = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Parameters")
	APawn* Instigator = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Parameters")
	ESpawnActorCollisionHandlingMethod SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Parameters")
	ESpawnActorScaleMethod TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

public:
	FActorSpawnParameters ConvertToEngineType() const;
};
