#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "BloodlustAwakening/Core/Team/CppTeam.h"

#include "CppTeamComponent.generated.h"

UCLASS(BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppTeamComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	TObjectPtr<UCppTeam> Team;
};
