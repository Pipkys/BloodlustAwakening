#include "BloodlustAwakening/Components/StatsComponent/CppStatsComponent.h"

void UCppStatsComponent::CacheStats(ECppStatsType InType, UDataTable* InDatabase, FCppStats* InStats)
{
	if (FCppStatsCache* StatsCache = CachedStats.Find(InType))
	{
		UE_LOG(LogTemp,
			Error,
			TEXT("UCppStatsComponent::CacheStats: stats of type '%s' already cached, owner is '%s'"),
			*StaticEnum<ECppStatsType>()->GetDisplayNameTextByValue((int64)InType).ToString(),
			*GetOwner()->GetName());
		return;
	}
	FCppStatsCache Cache;
	Cache.Database = InDatabase;
	Cache.Stats = InStats;
	CachedStats.Emplace(InType, Cache);
}

FCppStats* UCppStatsComponent::GetCachedStats(ECppStatsType InType) const
{
	const FCppStatsCache* StatsCache = CachedStats.Find(InType);
	if (!StatsCache)
	{
		UE_LOG(LogTemp,
			Error,
			TEXT("UCppStatsComponent::GetCachedStats: stats of type '%s' is not cached, owner is '%s'"),
			*StaticEnum<ECppStatsType>()->GetDisplayNameTextByValue((int64)InType).ToString(),
			*GetOwner()->GetName());
		return nullptr;
	}
	return StatsCache->Stats;
}

FCppStats* UCppStatsComponent::FindCachedStats(ECppStatsType InType) const
{
	const FCppStatsCache* StatsCache = CachedStats.Find(InType);
	return StatsCache ? StatsCache->Stats : nullptr;
}
