#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "NiagaraSystem.h"

#include "BloodlustAwakening/Components/AttackComponent/CppAttackType.h"
#include "BloodlustAwakening/Core/SpawnParameters/CppActorSpawnParameters.h"

#include "CppAttackComponent.generated.h"

UCLASS(BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	ECppAttackType AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Effects")
	TObjectPtr<UNiagaraSystem> AttackEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Range;

private:
	mutable FTimerHandle AttackTimer;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackTimerCallback);
	UPROPERTY()
	FAttackTimerCallback AttackTimerCallback = {};

public:
	virtual void SetupComponent(AActor* InOwner, const FCppActorSpawnParameters& InSpawnParameters);

public:
	UFUNCTION(BlueprintCallable, Category = "Attack")
	bool IsTargetInAttackRange(AActor* InTarget, const FTransform& InAttackPointTransform) const;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	bool IsCanAttack() const;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	bool IsCanAttackTarget(AActor* InTarget, const FTransform& InAttackPointTransform) const;

public:
	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual void PerformAttack(AActor* InTarget, const FTransform& InAttackPointTransform) PURE_VIRTUAL(UCppAttackComponent::PerformAttack);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SetAttackTimerCallaback(UObject* InObject, const FName& InFunctionName);

protected:
	void OnPerformAttack() const;
};

template<typename T>
concept IsAttackComponent = std::is_base_of_v<UCppAttackComponent, T>;
