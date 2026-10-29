#pragma once

#include "CoreMinimal.h"

#include "CppAttackType.generated.h"

UENUM(BlueprintType)
enum class ECppAttackType : uint8
{
	Melee = 0 UMETA(DisplayName = "Melee"),
	Ranged UMETA(DisplayName = "Ranged"),
};
