#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Actors/Interface/CppEntity.h"
#include "BloodlustAwakening/Core/Stats/CppStats.h"

#include "CppStatsSubsystem.generated.h"

UCLASS(BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppStatsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	TMap<ECppStatsType, TObjectPtr<UDataTable>> StatsDatabase;

public:
	FCppStats* LoadStats(TScriptInterface<ICppEntity> InUnit, ECppStatsType InType);
};
