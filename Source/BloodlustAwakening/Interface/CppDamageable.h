#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "UObject/Interface.h"

#include "BloodlustAwakening/Components/HealthComponent/CppHealthComponent.h"

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
	virtual float GetMaxHealth() const = 0;
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void SetMaxHealth(float InMaxHealth) = 0;

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual float GetCurrentHealth() const = 0;

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void SetCurrentHealth(float InCurrentHealth) = 0;

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual bool IsAlive() const = 0;

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual bool IsDead() const = 0;

public:
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void SubscribeToOnTakeDamageEvent(UObject* InObject, const FName& InFunctionName) = 0;

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void SubscribeToOnDieEvent(UObject* InObject, const FName& InFunctionName) = 0;

public:
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void Die() = 0;
};
