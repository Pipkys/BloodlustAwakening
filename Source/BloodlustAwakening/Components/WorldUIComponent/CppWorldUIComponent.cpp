#include "BloodlustAwakening/Components/WorldUIComponent/CppWorldUIComponent.h"

#include "BloodlustAwakening/Components/HealthComponent/CppHealthComponent.h"
#include "BloodlustAwakening/Components/AttackComponent/CppAttackComponent.h"
#include "BloodlustAwakening/Interface/CppDamageable.h"
#include "BloodlustAwakening/Interface/CppAttacker.h"

#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UCppWorldUIComponent::UCppWorldUIComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f;
}

void UCppWorldUIComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ICppDamageable* DamageableInterface = Cast<ICppDamageable>(GetOwner()))
	{
		HealthComponent = DamageableInterface->GetHealthComponent();
	}

	if (ICppAttacker* AttackerInterface = Cast<ICppAttacker>(GetOwner()))
	{
		AttackComponent = AttackerInterface->GetAttackComponent();
	}

	if (UUserWidget* UserWidgetObject = GetUserWidgetObject())
	{
		HealthBar = Cast<UProgressBar>(UserWidgetObject->GetWidgetFromName(TEXT("HealthBar")));
		DamageText = Cast<UTextBlock>(UserWidgetObject->GetWidgetFromName(TEXT("DamageText")));
		AttackSpeedText = Cast<UTextBlock>(UserWidgetObject->GetWidgetFromName(TEXT("AttackSpeedText")));

		if (!HealthBar || !DamageText || !AttackSpeedText)
		{
			UE_LOG(LogTemp, Error,
				TEXT("UCppWorldUIComponent::BeginPlay: widget lookup failed on '%s': HealthBar = '%s', DamageText = '%s', AttackSpeedText= '%s'. ")
				TEXT("Check WBP: name in Details must match exactly (press Enter after typing), and the widget type must be 'Text' (UTextBlock), not 'Text Box'."),
				*GetNameSafe(GetOwner()),
				HealthBar ? TEXT("OK") : TEXT("NOT FOUND"),
				DamageText ? TEXT("OK") : TEXT("NOT FOUND"),
				AttackSpeedText ? TEXT("OK") : TEXT("NOT FOUND"));
		}
	}
}

void UCppWorldUIComponent::TickComponent(float InDeltaTime, ELevelTick InTickType, FActorComponentTickFunction* InTickFunction)
{
	Super::TickComponent(InDeltaTime, InTickType, InTickFunction);

	if (!HealthComponent || !AttackComponent)
	{
		return;
	}

	const float HealthPercent = HealthComponent->GetMaxHealth() > 0.f
		? HealthComponent->GetCurrentHealth() / HealthComponent->GetMaxHealth()
		: 0.f;
	const float CurrentDamage = AttackComponent->GetAttackParams().Damage;
	const float CurrentAttackSpeed = AttackComponent->GetAttackParams().Speed;

	if (!FMath::IsNearlyEqual(HealthPercent, LastHealthPercent, 0.001f)
		|| !FMath::IsNearlyEqual(CurrentDamage, LastDamage, 0.001f)
		|| !FMath::IsNearlyEqual(CurrentAttackSpeed, LastAttackSpeed, 0.001f))
	{
		UpdateHealthBar(HealthPercent);
		UpdateStatsDisplay(CurrentDamage, CurrentAttackSpeed);
		LastHealthPercent = HealthPercent;
		LastDamage = CurrentDamage;
		LastAttackSpeed = CurrentAttackSpeed;
	}
}

FLinearColor UCppWorldUIComponent::GetHealthColor(float InHealthPercent) const
{
	if (InHealthPercent >= MediumHealthThreshold)
	{
		return FullHealthColor;
	}
	else if (InHealthPercent >= LowHealthThreshold)
	{
		const float Alpha = (InHealthPercent - LowHealthThreshold) / (MediumHealthThreshold - LowHealthThreshold);
		return FLinearColor::LerpUsingHSV(MediumHealthColor, FullHealthColor, Alpha);
	}
	else
	{
		const float Alpha = InHealthPercent / LowHealthThreshold;
		return FLinearColor::LerpUsingHSV(LowHealthColor, MediumHealthColor, Alpha);
	}
}

void UCppWorldUIComponent::UpdateHealthBar(float InHealthPercent)
{
	HealthBar->SetPercent(InHealthPercent);
	HealthBar->SetFillColorAndOpacity(GetHealthColor(InHealthPercent));
}

void UCppWorldUIComponent::UpdateStatsDisplay(float InDamage, float InAttackSpeed)
{
	DamageText->SetText(FText::AsNumber(InDamage));
	AttackSpeedText->SetText(FText::AsNumber(InAttackSpeed));
}
