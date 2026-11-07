#pragma once

#include "CoreMinimal.h"

#include "CppAttackParams.generated.h"

class UCppAttackBehavior;
class UNiagaraSystem;
class UDamageType;

USTRUCT(BlueprintType)
struct FCppAttackParams
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<UCppAttackBehavior> AttackBehavior;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Range = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Speed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TObjectPtr<UNiagaraSystem> AttackEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TObjectPtr<UNiagaraSystem> ImpactEffect;
};
