#include "BloodlustAwakening/Components/HealthComponent/CppHealthComponent.h"

#include "BloodlustAwakening/Components/HealthComponent/CppHealthComponentStats.h"
#include "BloodlustAwakening/Subsystems/CppStatsSubsystem.h"

void UCppHealthComponent::SetupComponent(AActor* InOwner, const FCppActorSpawnParameters& InSpawnParameters)
{
	if (FCppStats* HealthStats = GetWorld()->GetSubsystem<UCppStatsSubsystem>()->LoadStats(InOwner, ECppStatsType::Health))
	{
		FCppHealthComponentStats* Stats = (FCppHealthComponentStats*)HealthStats;
		MaxHealth = Stats->MaxHealth;
		CurrentHealth = MaxHealth;
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("UCppHealthComponent::SetupComponent: Failed to load health stats"));
}

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
	CurrentHealth = InCurrentHealth;
}

void UCppHealthComponent::OnTakeDamage(float InBaseDamage,
	const FDamageEvent& InDamageEvent,
	AController* InInstigatorController,
	AActor* InDamageCauser)
{
	FCppDamageEvent DamageEvent;
	DamageEvent.Damage = InBaseDamage;
	DamageEvent.DamageEvent = &InDamageEvent;
	DamageEvent.InstigatorController = InInstigatorController;
	DamageEvent.DamageCauser = InDamageCauser;
	OnTakeDamageEvent.Broadcast(DamageEvent);
}

void UCppHealthComponent::OnDie()
{
	if (IsAlive())
	{
		bDead = true;
		OnDieEvent.Broadcast();
	}
}

void UCppHealthComponent::SubscribeToOnTakeDamageEvent(UObject* InObject, const FName& InFunctionName)
{
	FScriptDelegate Delegate;
	Delegate.BindUFunction(InObject, InFunctionName);
	OnTakeDamageEvent.Add(Delegate);
}

void UCppHealthComponent::SubscribeToOnDieEvent(UObject* InObject, const FName& InFunctionName)
{
	FScriptDelegate Delegate;
	Delegate.BindUFunction(InObject, InFunctionName);
	OnDieEvent.Add(Delegate);
}
