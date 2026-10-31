#include "BloodlustAwakening/Components/HealthComponent/CppHealthComponent.h"

#include "BloodlustAwakening/Components/HealthComponent/CppHealthStats.h"
#include "BloodlustAwakening/Components/StatsComponent/CppStatsComponent.h"
#include "BloodlustAwakening/Core/Stats/CppStats.h"
#include "BloodlustAwakening/Subsystems/CppStatsSubsystem.h"

void UCppHealthComponent::SetupComponent(AActor* InOwner, const FCppActorSpawnParameters& InSpawnParameters)
{
	FCppHealthStats* Stats = UCppStatsSubsystem::LoadCppStatsAs<ECppStatsType::Health>(InOwner);
	if (!Stats)
	{
		UE_LOG(LogTemp, Error, TEXT("UCppHealthComponent::SetupComponent: failed to load health stats for '%s'"), *GetNameSafe(InOwner));
		return;
	}
	MaxHealth = Stats->MaxHealth;
	CurrentHealth = MaxHealth;

	//// Upgrade/buff modifiers recompute the effective max health.
	//if (AActor* Owner = GetOwner())
	//{
	//	if (UCppStatsComponent* StatsComponent = Owner->FindComponentByClass<UCppStatsComponent>())
	//	{
	//		StatsComponent->OnStatsChanged.AddDynamic(this, &UCppHealthComponent::HandleStatsChanged);
	//	}
	//}
}

//void UCppHealthComponent::RefreshMaxHealth()
//{
//	const AActor* Owner = GetOwner();
//	const UCppStatsComponent* StatsComponent = Owner ? Owner->FindComponentByClass<UCppStatsComponent>() : nullptr;
//
//	MaxHealth = FMath::Max(1.0f, StatsComponent
//		? StatsComponent->GetStatValue(CppStatIds::MaxHealth, BaseMaxHealth)
//		: BaseMaxHealth);
//
//	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);
//}

float UCppHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UCppHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

bool UCppHealthComponent::IsAlive() const
{
	return !bDead;
}

bool UCppHealthComponent::IsDead() const
{
	return bDead;
}

void UCppHealthComponent::SetMaxHealth(float InMaxHealth)
{
	MaxHealth = InMaxHealth;
}

void UCppHealthComponent::SetCurrentHealth(float InCurrentHealth)
{
	CurrentHealth = FMath::Clamp(InCurrentHealth, 0.0f, MaxHealth);
}

float UCppHealthComponent::OnTakeDamage(float InBaseDamage,
	const FDamageEvent& InDamageEvent,
	AController* InInstigatorController,
	AActor* InDamageCauser)
{
	if (IsDead())
	{
		return 0.f;
	}
	const float AppliedDamage = FMath::Max(0.f, InBaseDamage);
	CurrentHealth = FMath::Clamp(CurrentHealth - AppliedDamage, 0.f, MaxHealth);
	FCppDamageEvent DamageEvent;
	DamageEvent.Damage = AppliedDamage;
	DamageEvent.DamageEvent = &InDamageEvent;
	DamageEvent.InstigatorController = InInstigatorController;
	DamageEvent.DamageCauser = InDamageCauser;
	OnTakeDamageEvent.Broadcast(DamageEvent);
	if (CurrentHealth <= 0.f)
	{
		bDead = true;
		OnDieEvent.Broadcast();
	}
	return AppliedDamage;
}

void UCppHealthComponent::SubscribeToOnTakeDamageEvent(UObject* InObject, const FName& InFunctionName)
{
	FScriptDelegate Delegate;
	Delegate.BindUFunction(InObject, InFunctionName);
	OnTakeDamageEvent.AddUnique(Delegate);
}

void UCppHealthComponent::SubscribeToOnDieEvent(UObject* InObject, const FName& InFunctionName)
{
	FScriptDelegate Delegate;
	Delegate.BindUFunction(InObject, InFunctionName);
	OnDieEvent.AddUnique(Delegate);
}
