#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Components/AttackComponent/CppAttackBehavior.h"
#include "BloodlustAwakening/Components/AttackComponent/CppAttackParams.h"
#include "BloodlustAwakening/Core/SpawnParameters/CppActorSpawnParameters.h"

#include "Components/ActorComponent.h"

#include "CppAttackComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCppAttackComponent();

private:
	UPROPERTY()
	TObjectPtr<UCppAttackBehavior> AttackBehavior;
	const FCppAttackParams* AttackParams;
	FTimerHandle AttackTimer;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackTimerCallback);
	UPROPERTY()
	FAttackTimerCallback AttackTimerCallback = {};

private:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void SetupComponent(AActor* InOwner, const FCppActorSpawnParameters& InSpawnParameters);

public:
	UFUNCTION(BlueprintCallable, Category = "Attack")
	bool IsTargetInAttackRange(AActor* InTarget, const FTransform& InAttackPointTransform) const;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	bool IsCanAttack() const;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	bool IsCanAttackTarget(AActor* InTarget, const FTransform& InAttackPointTransform) const;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	const FCppAttackParams& GetAttackParams() const;

public:
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void PerformAttack(AActor* InTarget, const FTransform& InAttackPointTransform);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SetAttackTimerCallback(UObject* InObject, const FName& InFunctionName);

private:
	void StartAttackCooldown();
	void OnAttackCooldownFinished();
};
