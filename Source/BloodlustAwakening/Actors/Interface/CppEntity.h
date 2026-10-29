#pragma once

#include "CoreMinimal.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "UObject/Interface.h"

#include "BloodlustAwakening/Components/StatsComponent/CppStatsComponent.h"
#include "BloodlustAwakening/Components/TeamComponent/CppTeamComponent.h"
#include "BloodlustAwakening/Core/Stats/CppStats.h"
#include "BloodlustAwakening/Core/Team/CppTeam.h"

#include "CppEntity.generated.h"

UINTERFACE(NotBlueprintable, BlueprintType)
class BLOODLUSTAWAKENING_API UCppEntity : public UInterface
{
    GENERATED_BODY()
};

class BLOODLUSTAWAKENING_API ICppEntity
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Visual")
    virtual UStaticMeshComponent* GetStaticMeshComponent() = 0;

    UFUNCTION(BlueprintCallable, Category = "Team")
    virtual UCppTeamComponent* GetTeamComponent() = 0;

    UFUNCTION(BlueprintCallable, Category = "Stats")
    virtual UCppStatsComponent* GetStatsComponent() = 0;

    UFUNCTION(BlueprintCallable, Category = "Team")
    virtual UCppTeam* GetTeam() const = 0;

    UFUNCTION(BlueprintCallable, Category = "Team")
    virtual bool IsHostileTo(TScriptInterface<ICppEntity> InOther) const = 0;

    virtual FCppStats* GetStats(ECppStatsType InType) = 0;
    virtual void CacheStats(const FCppStatsView& InStatsView) = 0;
};
