#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"

#include "BloodlustAwakening/Buildings/Implementation/CppBuilding.h"

#include "CppPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API ACppPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACppPlayerController();

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	int32 MappingPriority = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_PrimaryPressed;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Pan;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Zoom;

private:
	TSubclassOf<ACppBuilding> CurrentBuildingBlueprint;

	UPROPERTY()
	mutable TArray<TObjectPtr<ACppCell>> MarkedCells;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

private:
	void OnPrimaryPressed(const FInputActionValue& Value);
	void OnPan(const FInputActionValue& Value);
	void OnZoom(const FInputActionValue& Value);

public:
	UFUNCTION(BlueprintCallable, Category = "Build")
	void SetBuildingBlueprint(TSubclassOf<ACppBuilding> InBuildingBlueprint);

private:
	bool IsBuildingBlueprintSelected() const;
	void MarkCellsForBuildingBlueprint() const;
	void BuildSelectedBuilding() const;
};