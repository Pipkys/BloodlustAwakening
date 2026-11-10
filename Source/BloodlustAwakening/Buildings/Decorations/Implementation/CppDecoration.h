#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Buildings/Implementation/CppGridBuilding.h"

#include "CppDecoration.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API ACppDecoration : public ACppGridBuilding
{
	GENERATED_BODY()

public:
	ACppDecoration();
};
