#include "BloodlustAwakening/Components/WorldUIComponent/CppWorldUIComponent.h"

#include "BloodlustAwakening/Components/HealthComponent/CppHealthComponent.h"
#include "BloodlustAwakening/Components/AttackComponent/CppAttackComponent.h"
#include "BloodlustAwakening/Interface/CppDamageable.h"
#include "BloodlustAwakening/Interface/CppAttacker.h"

#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

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
		HealthComp = DamageableInterface->GetHealthComponent();
	}

	if (ICppAttacker* AttackerInterface = Cast<ICppAttacker>(GetOwner()))
	{
		AttackComp = AttackerInterface->GetAttackComponent();
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

void UCppWorldUIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!HealthComp || !AttackComp)
	{
		return;
	}

	const float HealthPercent = HealthComp->GetMaxHealth() > 0.f
		? HealthComp->GetCurrentHealth() / HealthComp->GetMaxHealth()
		: 0.f;
	const float CurrentDamage = AttackComp->GetDamage();
	const float CurrentAttackSpeed = AttackComp->GetSpeed();

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

FLinearColor UCppWorldUIComponent::GetHealthColor(float HealthPercent) const
{
	if (HealthPercent >= MediumHealthThreshold)
	{
		return FullHealthColor;
	}
	else if (HealthPercent >= LowHealthThreshold)
	{
		const float Alpha = (HealthPercent - LowHealthThreshold) / (MediumHealthThreshold - LowHealthThreshold);
		return FLinearColor::LerpUsingHSV(MediumHealthColor, FullHealthColor, Alpha);
	}
	else
	{
		const float Alpha = HealthPercent / LowHealthThreshold;
		return FLinearColor::LerpUsingHSV(LowHealthColor, MediumHealthColor, Alpha);
	}
}

void UCppWorldUIComponent::UpdateHealthBar(float HealthPercent)
{
	HealthBar->SetPercent(HealthPercent);
	HealthBar->SetFillColorAndOpacity(GetHealthColor(HealthPercent));
}

void UCppWorldUIComponent::UpdateStatsDisplay(float Damage, float AttackSpeed)
{
	DamageText->SetText(FText::AsNumber(Damage));
	AttackSpeedText->SetText(FText::AsNumber(AttackSpeed));
}
