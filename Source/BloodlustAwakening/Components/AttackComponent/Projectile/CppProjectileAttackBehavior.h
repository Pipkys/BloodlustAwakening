#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Components/AttackComponent/CppAttackBehavior.h"

#include "CppProjectileAttackBehavior.generated.h"

UCLASS(BlueprintType, Blueprintable)
class UCppProjectileAttackBehavior : public UCppAttackBehavior
{
    GENERATED_BODY()

public:
    virtual void PerformAttack(AActor* InTarget, const FTransform& InAttackPoint) override;
};
