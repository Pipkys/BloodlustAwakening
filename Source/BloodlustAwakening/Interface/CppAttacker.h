#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Components/AttackComponent/CppAttackComponent.h"

#include "GameFramework/Actor.h"
#include "UObject/Interface.h"

#include "CppAttacker.generated.h"

class USphereComponent;

UINTERFACE(NotBlueprintable, BlueprintType)
class BLOODLUSTAWAKENING_API UCppAttacker : public UInterface
{
    GENERATED_BODY()
};

class BLOODLUSTAWAKENING_API ICppAttacker
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Attack")
    virtual UCppAttackComponent* GetAttackComponent() = 0;

    UFUNCTION(BlueprintCallable, Category = "Attack")
    virtual USphereComponent* GetAttackPointComponent() = 0;

    UFUNCTION(BlueprintCallable, Category = "Target")
    virtual AActor* GetCurrentTarget() const = 0;

    UFUNCTION(BlueprintCallable, Category = "Target")
    virtual void LockOnTarget(AActor* InTarget) = 0;

    UFUNCTION(BlueprintCallable, Category = "Target")
    virtual void ReleaseTarget() = 0;

    UFUNCTION(BlueprintCallable, Category = "Attack")
    virtual bool IsTargetInAttackRange(AActor* InTarget, const FTransform& InAttackPointTransform) const;

    UFUNCTION(BlueprintCallable, Category = "Attack")
    virtual bool IsCanAttack() const;

    UFUNCTION(BlueprintCallable, Category = "Attack")
    virtual bool IsCanAttackTarget(AActor* InTarget, const FTransform& InAttackPointTransform) const;

protected:
    UCppAttackComponent* ResolveAttackComponent() const
    {
        return const_cast<ICppAttacker*>(this)->GetAttackComponent();
    }
};

inline bool ICppAttacker::IsTargetInAttackRange(AActor* InTarget, const FTransform& InAttackPointTransform) const
{
    return ResolveAttackComponent()->IsTargetInAttackRange(InTarget, InAttackPointTransform);
}

inline bool ICppAttacker::IsCanAttack() const
{
    return ResolveAttackComponent()->IsCanAttack();
}

inline bool ICppAttacker::IsCanAttackTarget(AActor* InTarget, const FTransform& InAttackPointTransform) const
{
    return ResolveAttackComponent()->IsCanAttackTarget(InTarget, InAttackPointTransform);
}
