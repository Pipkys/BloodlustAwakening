#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Actors/Units/CppUnit.h"

#include "CppRangedUnit.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API ACppRangedUnit : public ACppUnit
{
	GENERATED_BODY()

public:
	ACppRangedUnit();

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

public:
	void Setup(const FCppUnitSpawnParameters& InSpawnParameters) override;
};

