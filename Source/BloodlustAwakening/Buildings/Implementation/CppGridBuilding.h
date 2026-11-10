#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Interface/CppGridPlaceable.h"
#include "BloodlustAwakening/Level/Grid/CppCell.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "NavModifierComponent.h"

#include "CppGridBuilding.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API ACppGridBuilding : public APawn, public ICppGridPlaceable
{
	GENERATED_BODY()

public:
	ACppGridBuilding();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Components")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Components")
	TObjectPtr<UNavModifierComponent> NavModifierComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|GridData")
	TArray<ACppCell*> OccupiedCells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|GridData")
	int32 SizeInCells;

protected:
	bool bStaticBuilding = false;

protected:
	virtual void PreInitializeComponents() override;

public:
	int32 GetSizeInCells() const override;
	TArray<ACppCell*>& GetOccupiedCells() override;

protected:
	void EnableBoxBody(bool bAffectsNavigation);
	void RestoreActorDefaults();
};
