#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "BloodlustAwakening/Core/Events/CppDamageEvent.h"
#include "BloodlustAwakening/Core/SpawnParameters/CppActorSpawnParameters.h"

#include "CppHealthComponent.generated.h"

UCLASS(BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHealth = 0.f;

	bool bDead = false;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTakeDamage, FCppDamageEvent, InDamageEvent);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	FOnTakeDamage OnTakeDamageEvent;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDie, UCppHealthComponent*, InHealthComponent);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	FOnDie OnDieEvent;

public:
	void SetupComponent(AActor* InOwner, const FCppActorSpawnParameters& InSpawnParameters);

public:
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsDead() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetMaxHealth(float InMaxHealth);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float InCurrentHealth);

public:
	UFUNCTION(BlueprintCallable, Category = "Health")
	float OnTakeDamage(float InBaseDamage,
		const FDamageEvent& InDamageEvent,
		AController* InInstigatorController,
		AActor* InDamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void OnDie();

public:
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SubscribeToOnTakeDamageEvent(UObject* InObject, const FName& InFunctionName);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SubscribeToOnDieEvent(UObject* InObject, const FName& InFunctionName);
};
