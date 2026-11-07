#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Components/AttackComponent/CppAttackBehavior.h"

#include "CppMeleeAttackBehavior.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLOODLUSTAWAKENING_API UCppMeleeAttackBehavior : public UCppAttackBehavior
{
	GENERATED_BODY()

public:
	virtual void PerformAttack(AActor* InTarget, const FTransform& InAttackPoint) override;

protected:
	void AppendCleaveTargets(AActor* InPrimaryTarget, TArray<AActor*>& InOutTargets) const;
};