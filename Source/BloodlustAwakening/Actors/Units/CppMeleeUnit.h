#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Actors/Units/CppUnit.h"

#include "CppMeleeUnit.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API ACppMeleeUnit : public ACppUnit
{
	GENERATED_BODY()

public:
	ACppMeleeUnit();

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

public:
	void Setup(const FCppUnitSpawnParameters& InSpawnParameters) override;
};

