#include "BloodlustAwakening/Subsystems/CppStatsSubsystem.h"

#include "BloodlustAwakening/Components/StatsComponent/CppStatsComponent.h"

FCppStats* UCppStatsSubsystem::LoadStats(TScriptInterface<ICppEntity> InEntity, ECppStatsType InType)
{
	UObject* EntityObject = InEntity.GetObject();
	ICppEntity* EntityInterface = InEntity.GetInterface();
	if (!EntityObject || !EntityInterface)
	{
		UE_LOG(LogTemp, Error, TEXT("UCppStatsSubsystem::LoadStats: entity is invalid or does not implement 'ICppEntity'"));
		return nullptr;
	}
	UCppStatsComponent* StatsComponent = EntityInterface->GetStatsComponent();
	if (!StatsComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("UCppStatsSubsystem::LoadStats: '%s' has no stats component"), *EntityObject->GetName());
		return nullptr;
	}
	if (FCppStats* Loaded = StatsComponent->GetCachedStats(InType))
	{
		return Loaded;
	}
	TObjectPtr<UDataTable>* FoundStatsDatabase = StatsDatabases.Find(InType);
	if (!FoundStatsDatabase)
	{
		UE_LOG(LogTemp,
			Error,
			TEXT("UCppStatsSubsystem::LoadStats: stats database is not set for '%s'"),
			*StaticEnum<ECppStatsType>()->GetDisplayNameTextByValue((int64)InType).ToString());
		return nullptr;
	}
	FName RowName = FCppStats::GetArchetypeRowName(EntityObject->GetClass());
	FCppStats* EntityStats = (*FoundStatsDatabase)->FindRow<FCppStats>(RowName, TEXT("UCppStatsSubsystem::LoadStats"), true);
	if (!EntityStats)
	{
		UE_LOG(LogTemp,
			Error,
			TEXT("UCppStatsSubsystem::LoadStats: no row '%s' for '%s', database: '%s'"),
			*RowName.ToString(),
			*EntityObject->GetName(),
			*FoundStatsDatabase->GetName());
		return nullptr;
	}
	StatsComponent->CacheStats(InType, *FoundStatsDatabase, EntityStats);
	return EntityStats;
}
