#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Core/Stats/CppStats.h"

#include "CppHealthComponentStats.generated.h"

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppHealthComponentStatsDatabaseRow : public FCppStatsDatabaseRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
};

using FCppHealthComponentStats = FCppHealthComponentStatsDatabaseRow;
