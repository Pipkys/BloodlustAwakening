#pragma once

#include "CoreMinimal.h"

#include "UObject/Interface.h"

#include "BloodlustAwakening/Level/Grid/CppCell.h"

#include "CppGridPlaceable.generated.h"

UINTERFACE(NotBlueprintable, BlueprintType)
class BLOODLUSTAWAKENING_API UCppGridPlaceable : public UInterface
{
	GENERATED_BODY()
};

class BLOODLUSTAWAKENING_API ICppGridPlaceable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Data")
	virtual int32 GetSizeInCells() const = 0;

	UFUNCTION(BlueprintCallable, Category = "Data")
	virtual TArray<ACppCell*>& GetOccupiedCells() = 0;
};
