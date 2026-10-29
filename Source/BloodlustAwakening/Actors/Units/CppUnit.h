#pragma once

#include "CoreMinimal.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "BloodlustAwakening/Actors/Interface/CppAttacker.h"
#include "BloodlustAwakening/Actors/Interface/CppDamageable.h"
#include "BloodlustAwakening/Actors/Interface/CppEntity.h"
#include "BloodlustAwakening/Actors/Interface/CppMoveable.h"
#include "BloodlustAwakening/Actors/Units/CppUnitSpawnParameters.h"
#include "BloodlustAwakening/Components/AttackComponent/CppAttackComponent.h"
#include "BloodlustAwakening/Components/HealthComponent/CppHealthComponent.h"
#include "BloodlustAwakening/Components/StatsComponent/CppStatsComponent.h"
#include "BloodlustAwakening/Components/TeamComponent/CppTeamComponent.h"
#include "BloodlustAwakening/Core/Stats/CppStats.h"
#include "BloodlustAwakening/Core/Team/CppTeam.h"
#include "BloodlustAwakening/Subsystems/CppStatsSubsystem.h"
#include "BloodlustAwakening/Subsystems/CppTeamSubsystem.h"

#include "CppUnit.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Target")
	TObjectPtr<AActor> CurrentTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Detection")
	float DetectionRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Target")
	bool bLockedOnTarget;

protected:
	UPROPERTY()
	TObjectPtr<UCppTeamSubsystem> TeamSubsystem;

	UPROPERTY()
	TObjectPtr<UCppStatsSubsystem> StatsSubsystem;
	 
protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

public:
	virtual void Setup(const FCppUnitSpawnParameters& InSpawnParameters);

public:
	UStaticMeshComponent* GetStaticMeshComponent() override;
	UCppTeamComponent* GetTeamComponent() override;
	UCppStatsComponent* GetStatsComponent() override;
	UCppTeam* GetTeam() const override;
	bool IsHostileTo(TScriptInterface<ICppEntity> InOther) const override;
	FCppStats* GetStats(ECppStatsType InStatsType) override;
	void CacheStats(const FCppStatsView& InStatsView) override;

public:
	float TakeDamage(float InBaseDamage,
		struct FDamageEvent const& InDamageEvent,
		class AController* InEventInstigator,
		AActor* InDamageCauser) override;

public:
	UCppHealthComponent* GetHealthComponent() override;
	float GetMaxHealth() const override;
	void SetMaxHealth(float InMaxHealth) override;
	float GetCurrentHealth() const override;
	void SetCurrentHealth(float InCurrentHealth) override;
	bool IsAlive() const override;
	bool IsDead() const override;
	void SubscribeToOnTakeDamageEvent(UObject* InObject, const FName& InFunctionName) override;
	void SubscribeToOnDieEvent(UObject* InObject, const FName& InFunctionName) override;
	void Die() override;

public:
	UCppAttackComponent* GetAttackComponent() override;
	USphereComponent* GetAttackPointComponent() override;
	AActor* GetCurrentTarget() const override;
	void LockOnTarget(AActor* InTarget) override;
	void ReleaseTarget() override;
	bool IsTargetInAttackRange(AActor* InTarget, const FTransform& InAttackPointTransform) const override;
	bool IsCanAttack() const override;
	bool IsCanAttackTarget(AActor* InTarget, const FTransform& InAttackPointTransform) const override;
};
