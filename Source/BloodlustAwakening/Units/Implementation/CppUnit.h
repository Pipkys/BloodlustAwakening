#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Interface/CppAttacker.h"
#include "BloodlustAwakening/Interface/CppDamageable.h"
#include "BloodlustAwakening/Interface/CppEntity.h"
#include "BloodlustAwakening/Interface/CppMoveable.h"
#include "BloodlustAwakening/Units/Implementation/CppUnitSpawnParameters.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"

#include "CppUnit.generated.h"

class ACppAIController;
class UCppAttackComponent;
class UCppHealthComponent;
class UCppStatsComponent;
class UCppTeamComponent;
class UCppTeamSubsystem;
class UCppWorldUIComponent;

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API ACppUnit :
	public ACharacter,
	public ICppEntity,
	public ICppDamageable,
	public ICppAttacker,
	public ICppMoveable
{
	GENERATED_BODY()

public:
	ACppUnit();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Components")
	TObjectPtr<ACppAIController> AIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Components")
	TObjectPtr<UCppTeamComponent> TeamComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Components")
	TObjectPtr<UCppStatsComponent> StatsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Components")
	TObjectPtr<UCppHealthComponent> HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Components")
	TObjectPtr<UCppAttackComponent> AttackComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Components")
	TObjectPtr<USphereComponent> AttackPointComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Components")
	TObjectPtr<UCppWorldUIComponent> WorldUIComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Target")
	TObjectPtr<AActor> CurrentTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Detection")
	float DetectionRange = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Target")
	bool bLockedOnTarget = false;

protected:
	UPROPERTY()
	TObjectPtr<UCppTeamSubsystem> TeamSubsystem;

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

public:
	virtual void Setup(const FCppUnitSpawnParameters& InSpawnParameters);

public:
	UStaticMeshComponent* GetStaticMeshComponent() override;
	UCppTeamComponent* GetTeamComponent() override;
	UCppStatsComponent* GetStatsComponent() override;
	bool IsHostileTo(TScriptInterface<ICppEntity> InOther) const override;

protected:
	void LoadStats() override;

public:
	float TakeDamage(float InBaseDamage,
		const FDamageEvent& InDamageEvent,
		AController* InEventInstigator,
		AActor* InDamageCauser) override;

public:
	UCppHealthComponent* GetHealthComponent() override;

public:
	UCppAttackComponent* GetAttackComponent() override;
	USphereComponent* GetAttackPointComponent() override;
	AActor* GetCurrentTarget() const override;
	void LockOnTarget(AActor* InTarget) override;
	void ReleaseTarget() override;
};
