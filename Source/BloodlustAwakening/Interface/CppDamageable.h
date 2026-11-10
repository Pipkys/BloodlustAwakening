#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Components/HealthComponent/CppHealthComponent.h"

#include "GameFramework/Actor.h"
#include "UObject/Interface.h"

#include "CppDamageable.generated.h"

UINTERFACE(NotBlueprintable, BlueprintType)
class BLOODLUSTAWAKENING_API UCppDamageable : public UInterface
{
    GENERATED_BODY()
};

class BLOODLUSTAWAKENING_API ICppDamageable
{
    GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Visual")
	virtual UCppHealthComponent* GetHealthComponent() = 0;

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void SetMaxHealth(float InMaxHealth);

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual float GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void SetCurrentHealth(float InCurrentHealth);

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual bool IsDead() const;

public:
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void SubscribeToOnTakeDamageEvent(UObject* InObject, const FName& InFunctionName);

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void SubscribeToOnDieEvent(UObject* InObject, const FName& InFunctionName);

public:
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void Die();

protected:
	UCppHealthComponent* ResolveHealthComponent() const
	{
		return const_cast<ICppDamageable*>(this)->GetHealthComponent();
	}
};

inline float ICppDamageable::GetMaxHealth() const
{
	return ResolveHealthComponent()->GetMaxHealth();
}

inline void ICppDamageable::SetMaxHealth(float InMaxHealth)
{
	GetHealthComponent()->SetMaxHealth(InMaxHealth);
}

inline float ICppDamageable::GetCurrentHealth() const
{
	return ResolveHealthComponent()->GetCurrentHealth();
}

inline void ICppDamageable::SetCurrentHealth(float InCurrentHealth)
{
	GetHealthComponent()->SetCurrentHealth(InCurrentHealth);
}

inline bool ICppDamageable::IsAlive() const
{
	return ResolveHealthComponent()->IsAlive();
}

inline bool ICppDamageable::IsDead() const
{
	return ResolveHealthComponent()->IsDead();
}

inline void ICppDamageable::SubscribeToOnTakeDamageEvent(UObject* InObject, const FName& InFunctionName)
{
	GetHealthComponent()->SubscribeToOnTakeDamageEvent(InObject, InFunctionName);
}

inline void ICppDamageable::SubscribeToOnDieEvent(UObject* InObject, const FName& InFunctionName)
{
	GetHealthComponent()->SubscribeToOnDieEvent(InObject, InFunctionName);
}

inline void ICppDamageable::Die()
{
	if (UCppHealthComponent* HealthComponent = GetHealthComponent())
	{
		HealthComponent->OnDie();
	}
}
