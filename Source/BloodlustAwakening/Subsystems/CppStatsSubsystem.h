#pragma once

#include "CoreMinimal.h"

#include "Subsystems/WorldSubsystem.h"

#include "BloodlustAwakening/Interface/CppEntity.h"
#include "BloodlustAwakening/Core/Stats/CppStats.h"

#include "CppStatsSubsystem.generated.h"

UCLASS(BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppStatsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TMap<ECppStatsType, TObjectPtr<UDataTable>> StatsDatabases;

public:
	template<ECppStatsType InStatsType>
	static typename TCppStatsTypeTraits<InStatsType>::Type* LoadCppStatsAs(TScriptInterface<ICppEntity> InEntity)
	{
		UWorld* World = InEntity.GetObject()->GetWorld();
		UCppStatsSubsystem* Subsystem = World ? World->GetSubsystem<UCppStatsSubsystem>() : nullptr;
		if (!Subsystem)
		{
			return nullptr;
		}
		return static_cast<typename TCppStatsTypeTraits<InStatsType>::Type*>(Subsystem->LoadStats(InEntity, InStatsType));
	}

	FCppStats* LoadStats(TScriptInterface<ICppEntity> InEntity, ECppStatsType InType);
};
