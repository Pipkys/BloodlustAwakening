#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Buildings/Implementation/CppGridBuilding.h"

#include "CppSpawner.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API ACppSpawner : public ACppGridBuilding
{
	GENERATED_BODY()

public:
	ACppSpawner();
};
