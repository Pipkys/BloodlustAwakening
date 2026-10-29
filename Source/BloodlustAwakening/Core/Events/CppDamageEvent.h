#pragma once

#include "CoreMinimal.h"

#include "CppDamageEvent.generated.h"

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppDamageEvent
{
	GENERATED_BODY()

public:
	float Damage;
	const FDamageEvent* DamageEvent;
	AController* InstigatorController;
	AActor* DamageCauser;
};
