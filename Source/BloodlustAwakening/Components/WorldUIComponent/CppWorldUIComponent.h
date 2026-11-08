#pragma once

#include "CoreMinimal.h"

#include "Components/WidgetComponent.h"

#include "CppWorldUIComponent.generated.h"

class UCppHealthComponent;
class UCppAttackComponent;
class UProgressBar;
class UTextBlock;

// BTODO: Provide class 'WorldUIUpdater' which updates one thing at one time, for example:
//	UCppWorldUIHealthUpdater --> GetHealthComp, UpdateHealth, Draw
//	UCppWorldUIAttackUpdater --> GetAttackComp, Update Attack/Speed, Draw
// etc...
// 
// And this Component would be storing TArray<UCppWorldUIUpdater> or like that...

UCLASS(BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppWorldUIComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UCppWorldUIComponent();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WorldUI")
	FVector2D WidgetOffset = FVector2D(0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WorldUI")
	FLinearColor FullHealthColor = FLinearColor::Green;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WorldUI")
	FLinearColor MediumHealthColor = FLinearColor::Yellow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WorldUI")
	FLinearColor LowHealthColor = FLinearColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WorldUI")
	float MediumHealthThreshold = 0.7f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WorldUI")
	float LowHealthThreshold = 0.35f;

private:
	UPROPERTY()
	TObjectPtr<UCppHealthComponent> HealthComponent;

	UPROPERTY()
	TObjectPtr<UCppAttackComponent> AttackComponent;

	UPROPERTY()
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY()
	TObjectPtr<UTextBlock> DamageText;

	UPROPERTY()
	TObjectPtr<UTextBlock> AttackSpeedText;

private:
	float LastHealthPercent = -1.f;
	float LastDamage = -1.f;
	float LastAttackSpeed = -1.f;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float InDeltaTime, ELevelTick InTickType, FActorComponentTickFunction* InTickFunction) override;

private:
	FLinearColor GetHealthColor(float InHealthPercent) const;
	void UpdateHealthBar(float InHealthPercent);
	void UpdateStatsDisplay(float InDamage, float InAttackSpeed);
};
