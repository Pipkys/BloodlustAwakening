#pragma once

#include "CoreMinimal.h"

#include "NiagaraSystem.h"

#include "BloodlustAwakening/Components/AttackComponent/CppAttackComponent.h"

#include "CppMeleeAttackComponent.generated.h"

UCLASS(BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppMeleeAttackComponent : public UCppAttackComponent
{
	GENERATED_BODY()

	using Super = UCppAttackComponent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Melee")
	bool bSingleTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Melee")
	float CleaveRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Melee")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

public:
	void SetupComponent(AActor* InOwner, const FCppActorSpawnParameters& InSpawnParameters) override;

public:
	void PerformAttack(AActor* InTarget, const FTransform& InAttackPointTransform) override;
};
