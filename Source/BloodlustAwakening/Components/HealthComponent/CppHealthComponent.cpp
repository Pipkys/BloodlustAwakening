#include "BloodlustAwakening/Components/HealthComponent/CppHealthComponent.h"

#include "BloodlustAwakening/Components/HealthComponent/CppHealthStats.h"
#include "BloodlustAwakening/Components/StatsComponent/CppStatsComponent.h"
#include "BloodlustAwakening/Core/Stats/CppStats.h"
#include "BloodlustAwakening/GameMode/CppGameplayStatics.h"
#include "BloodlustAwakening/Subsystems/CppStatsSubsystem.h"

void UCppHealthComponent::SetupComponent(AActor* InOwner, const FCppActorSpawnParameters& InSpawnParameters)
{
	FCppHealthStats* Stats = UCppStatsSubsystem::LoadStats<ECppStatsType::Health>(InOwner);
	if (!Stats)
	{
		UE_LOG(LogTemp, Error, TEXT("UCppHealthComponent::SetupComponent: failed to load health stats for '%s'"), *GetNameSafe(InOwner));
		return;
	}
	MaxHealth = Stats->MaxHealth;
	CurrentHealth = MaxHealth;
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
	CurrentHealth = FMath::Clamp(InCurrentHealth, 0.0f, MaxHealth);
}

float UCppHealthComponent::OnTakeDamage(float InBaseDamage,
	const FDamageEvent& InDamageEvent,
	AController* InInstigatorController,
	AActor* InDamageCauser)
{
	if (UCppGameplayStatics::IsInvincible(this) || IsDead())
	{
		return 0.f;
	}

	const float ActualDamage = FMath::Max(0.f, InBaseDamage);
	CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.f, MaxHealth);

	FCppDamageEvent DamageEvent;
	DamageEvent.Damage = ActualDamage;
	DamageEvent.DamageEvent = &InDamageEvent;
	DamageEvent.InstigatorController = InInstigatorController;
	DamageEvent.DamageCauser = InDamageCauser;
	OnTakeDamageEvent.Broadcast(DamageEvent);

	if (CurrentHealth <= 0.f)
	{
		OnDie();
	}

	return ActualDamage;
}

void UCppHealthComponent::OnDie()
{
	bDead = true;
	OnDieEvent.Broadcast(this);
	GetOwner()->Destroy();
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
