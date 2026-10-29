#pragma once

#include "CoreMinimal.h"

#include "CppGridType.generated.h"

UENUM(BlueprintType)
enum class ECppGridType : uint8
{
	Tile = 0 UMETA(DisplayName = "Tile"),
	Hex UMETA(DisplayName = "Hex")
};
