#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "BloodlustAwakening/Core/Stats/CppStats.h"

#include "CppStatsComponent.generated.h"

USTRUCT()
struct FCppStatsCache
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UDataTable> Database;
	FCppStats* Stats;
};

UCLASS(BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppStatsComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	TMap<ECppStatsType, FCppStatsCache> CachedStats;

public:
	void CacheStats(ECppStatsType InType, UDataTable* InDatabase, FCppStats* InStats);
	FCppStats* GetCachedStats(ECppStatsType InType) const;
	FCppStats* FindCachedStats(ECppStatsType InType) const;
};
