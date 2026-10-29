#include "BloodlustAwakening/Components/StatsComponent/CppStatsComponent.h"

FCppStats* UCppStatsComponent::GetStats(UClass* InClass, ECppStatsType InType)
{
	FCppStatsDatabaseRowView* Loaded = LoadedStats.FindByPredicate([InClass, InType](const FCppStatsDatabaseRowView& InStatsView)
	{
		return InStatsView.ClassBoundedTo == InClass && InStatsView.Type == InType;
	});

	return Loaded ? Loaded->Load() : nullptr;
}
