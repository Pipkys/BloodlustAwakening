#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "BloodlustAwakening/Core/Stats/CppStats.h"

#include "CppStatsComponent.generated.h"

UCLASS(BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FCppStatsDatabaseRowView> LoadedStats;

public:
	FCppStats* GetStats(UClass* InClass, ECppStatsType InType);
};
