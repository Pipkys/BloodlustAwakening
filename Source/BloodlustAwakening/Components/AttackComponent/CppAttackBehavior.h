#pragma once

#include "CoreMinimal.h"

#include "UObject/Object.h"
#include "StructUtils/InstancedStruct.h"

#include "CppAttackBehavior.generated.h"

class AActor;
class UNiagaraSystem;
struct FCppAttackParams;

UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class BLOODLUSTAWAKENING_API UCppAttackBehavior : public UObject
{
	GENERATED_BODY()

	AActor* Owner;
	const FCppAttackParams* Params;
protected:

public:
	void SetupBehavior(AActor* InOwner, const FInstancedStruct& InParams);

public:
	float GetRange() const;
	virtual void PerformAttack(AActor* InTarget, const FTransform& InAttackPoint) PURE_VIRTUAL(UCppAttackBehavior::PerformAttack)

protected:
	template<typename InType>
	InType* GetParamsAs() const
	{
		return (InType*)Params;
	}

	void SpawnNiagaraEffect(UNiagaraSystem* InEffect, const FTransform& InTransform) const;
};
