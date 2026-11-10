#pragma once

#include "CoreMinimal.h"

#include "Components/SphereComponent.h"

#include "BloodlustAwakening/Buildings/Implementation/CppBuilding.h"
#include "BloodlustAwakening/Buildings/Implementation/CppBuildingSpawnParameters.h"
#include "BloodlustAwakening/Interface/CppAttacker.h"
#include "BloodlustAwakening/Interface/CppDamageable.h"
#include "BloodlustAwakening/Interface/CppEntity.h"
#include "BloodlustAwakening/Components/AttackComponent/CppAttackComponent.h"
#include "BloodlustAwakening/Components/HealthComponent/CppHealthComponent.h"

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
	bool bLockedOnTarget = false;

protected:
	void Tick(float DeltaTime) override;

public:
	virtual void Setup(const FCppBuildingSpawnParameters& InSpawnParameters);

protected:
	void LoadStats() override;

public:
	float TakeDamage(float InBaseDamage,
		struct FDamageEvent const& InDamageEvent,
		class AController* InEventInstigator,
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
