#pragma once

#include "CoreMinimal.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

#include "BloodlustAwakening/Actors/Buildings/CppBuilding.h"
#include "BloodlustAwakening/Actors/Buildings/CppBuildingSpawnParameters.h"
#include "BloodlustAwakening/Actors/Interface/CppAttacker.h"
#include "BloodlustAwakening/Actors/Interface/CppDamageable.h"
#include "BloodlustAwakening/Actors/Interface/CppEntity.h"
#include "BloodlustAwakening/Components/AttackComponent/CppAttackComponent.h"
#include "BloodlustAwakening/Components/HealthComponent/CppHealthComponent.h"
#include "BloodlustAwakening/Components/StatsComponent/CppStatsComponent.h"
#include "BloodlustAwakening/Components/TeamComponent/CppTeamComponent.h"
#include "BloodlustAwakening/Core/Stats/CppStats.h"
#include "BloodlustAwakening/Core/Team/CppTeam.h"

#include "CppTower.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API ACppTower : public ACppBuilding, public ICppDamageable, public ICppAttacker
{
	GENERATED_BODY()

public:
	ACppTower();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Components")
	TObjectPtr<UCppHealthComponent> HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Components")
	TObjectPtr<UCppAttackComponent> AttackComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Components")
	TObjectPtr<USphereComponent> AttackPointComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Target")
	TObjectPtr<AActor> CurrentTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Attack")
	bool bLockedOnTarget;

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

public:
	virtual void Setup(const FCppBuildingSpawnParameters& InSpawnParameters);

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
