#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Actors/Interface/CppEntity.h"
#include "BloodlustAwakening/Core/Team/CppTeam.h"

#include "CppTeamSubsystem.generated.h"

UCLASS(BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppTeamSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	TArray<UCppTeam*> Teams;
	TArray<UCppTeamLink*> Links;

public:
	UFUNCTION(BlueprintCallable, Category = "Team")
	UCppTeam* GetTeam(const FName& InName);

	UFUNCTION(BlueprintCallable, Category = "Team")
	UCppTeamLink* GetTeamLink(TScriptInterface<ICppEntity> InFirstEntity, TScriptInterface<ICppEntity> InSecondEntity);

	UFUNCTION(BlueprintCallable, Category = "Team")
	bool IsFriendly(const TScriptInterface<ICppEntity> InFirstEntity, const TScriptInterface<ICppEntity> InSecondEntity) const;

	UFUNCTION(BlueprintCallable, Category = "Team")
	bool IsNeutral(const TScriptInterface<ICppEntity> InFirstEntity, const TScriptInterface<ICppEntity> InSecondEntity) const;

	UFUNCTION(BlueprintCallable, Category = "Team")
	bool IsFriendlyOrNeutral(const TScriptInterface<ICppEntity> InFirstEntity, const TScriptInterface<ICppEntity> InSecondEntity) const;

	UFUNCTION(BlueprintCallable, Category = "Team")
	bool IsHostile(const TScriptInterface<ICppEntity> InFirstEntity, const TScriptInterface<ICppEntity> InSecondEntity) const;

public:
	UFUNCTION(BlueprintCallable, Category = "Team")
	UCppTeam* CreateTeam(const FName& InName);

	UFUNCTION(BlueprintCallable, Category = "Team")
	UCppTeamLink* CreateTeamLink(UCppTeam* InFirstTeam, UCppTeam* InSecondTeam, ECppTeamRelationship InTeamRelationship);
};
