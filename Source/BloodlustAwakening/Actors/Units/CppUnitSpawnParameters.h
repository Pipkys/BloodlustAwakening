#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Core/SpawnParameters/CppActorSpawnParameters.h"
#include "BloodlustAwakening/Core/Team/CppTeam.h"

#include "CppUnitSpawnParameters.generated.h"

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppUnitSpawnParameters : public FCppActorSpawnParameters
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	TObjectPtr<UCppTeam> Team;
};
