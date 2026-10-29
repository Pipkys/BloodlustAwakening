#include "BloodlustAwakening/Subsystems/CppStatsSubsystem.h"

namespace BloodlustAwakening::Private
{
	static FCppStats* Internal_LoadStats(TScriptInterface<ICppEntity> InUnit, ECppStatsType InType)
	{
		FCppStats* Stats = InUnit->GetStats(InType);
		if (!Stats)
		{
			return nullptr;
		}
		UE_LOG(LogTemp, Warning, TEXT("Internal_LoadStats: Attempting to load already loaded stats"));
		return Stats;
	}
}

FCppStats* UCppStatsSubsystem::LoadStats(TScriptInterface<ICppEntity> InUnit, ECppStatsType InType)
{
	FCppStats* Stats = BloodlustAwakening::Private::Internal_LoadStats(InUnit, InType);
	if (!Stats)
	{
		FCppStatsView StatsView;
		StatsView.Type = InType;
		StatsView.ClassBoundedTo = InUnit.GetObject()->GetClass();
		StatsView.Database = *StatsDatabase.Find(InType);
		InUnit->CacheStats(StatsView);
		return StatsView.Load();
	}
	return Stats;
}
