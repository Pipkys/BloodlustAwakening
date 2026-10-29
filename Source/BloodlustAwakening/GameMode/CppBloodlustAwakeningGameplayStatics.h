#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Actors/Buildings/Towers/CppTower.h"
#include "BloodlustAwakening/Actors/Buildings/Towers/CppTowerSpawnParameters.h"
#include "BloodlustAwakening/Actors/Decorations/CppDecoration.h"
#include "BloodlustAwakening/Actors/Interface/CppDamageable.h"
#include "BloodlustAwakening/Actors/Interface/CppEntity.h"
#include "BloodlustAwakening/Actors/Projectiles/CppProjectile.h"
#include "BloodlustAwakening/Actors/Projectiles/CppProjectileSpawnParameters.h"
#include "BloodlustAwakening/Actors/Spawners/CppSpawner.h"
#include "BloodlustAwakening/Actors/Units/CppUnit.h"
#include "BloodlustAwakening/Actors/Units/CppUnitSpawnParameters.h"
#include "BloodlustAwakening/Core/Stats/CppStats.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "CppBloodlustAwakeningGameplayStatics.generated.h"

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FCppActorPredicate, AActor*, InActor);

UCLASS(Abstract, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppBloodlustAwakeningGameplayStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Health")
	static inline bool IsInvincible(TScriptInterface<ICppEntity> InEntity)
	{
		return !Cast<ICppDamageable>(InEntity.GetInterface());
	}

public:
	UFUNCTION(BlueprintCallable, Category = "Searching")
	static bool IsActorInRange(AActor* InActor,
		float InRange,
		const FVector& InLocation);

	UFUNCTION(BlueprintCallable, Category = "Searching", Meta = (WorldContext = "InWorldContextObject"))
	static AActor* FindActorByPredicate(UObject* InWorldContextObject,
		float InRange,
		const TArray<TEnumAsByte<ECollisionChannel>>& InActorTypes,
		UClass* InActorClassFilter,
		const TArray<AActor*>& InActorsToIgnore,
		const FVector& InOverlapLocation,
		const FCppActorPredicate& InPredicate);

	UFUNCTION(BlueprintCallable, Category = "Searching", Meta = (WorldContext = "InWorldContextObject"))
	static TArray<AActor*> FindActors(UObject* InWorldContextObject,
		float InRange,
		const TArray<TEnumAsByte<ECollisionChannel>>& InActorTypes,
		UClass* InActorClassFilter,
		const TArray<AActor*>& InActorsToIgnore,
		const FVector& InOverlapLocation);

	UFUNCTION(BlueprintCallable, Category = "Searching", Meta = (WorldContext = "InWorldContextObject"))
	static TArray<AActor*> FindActorsByPredicate(UObject* InWorldContextObject,
		float InRange,
		const TArray<TEnumAsByte<ECollisionChannel>>& InActorTypes,
		UClass* InActorClassFilter,
		const TArray<AActor*>& InActorsToIgnore,
		const FVector& InOverlapLocation,
		const FCppActorPredicate& InPredicate);

	UFUNCTION(BlueprintCallable, Category = "Searching")
	static AActor* FindNearestActor(const TArray<AActor*>& InActors,
		float InRange,
		const FVector& InLocation);

	UFUNCTION(BlueprintCallable, Category = "Searching")
	static AActor* FindNearestActorByPredicate(const TArray<AActor*>& InActors,
		float InRange,
		const FVector& InLocation,
		const FCppActorPredicate& InPredicate);

public:
	UFUNCTION(BlueprintCallable, Category = "Spawn", Meta = (WorldContext = "InWorldContextObject"))
	static ACppUnit* SpawnUnit(UObject* InWorldContextObject,
		TSubclassOf<ACppUnit> InClass,
		const FTransform& InTransform,
		const FCppUnitSpawnParameters& InSpawnParameters);

	UFUNCTION(BlueprintCallable, Category = "Spawn", Meta = (WorldContext = "InWorldContextObject"))
	static ACppDecoration* SpawnDecoration(UObject* InWorldContextObject,
		TSubclassOf<ACppDecoration> InClass,
		const FTransform& InTransform,
		const FCppActorSpawnParameters& InSpawnParameters);

	UFUNCTION(BlueprintCallable, Category = "Spawn", Meta = (WorldContext = "InWorldContextObject"))
	static ACppSpawner* SpawnSpawner(UObject* InWorldContextObject,
		TSubclassOf<ACppSpawner> InClass,
		const FTransform& InTransform,
		const FCppActorSpawnParameters& InSpawnParameters);

	UFUNCTION(BlueprintCallable, Category = "Spawn", Meta = (WorldContext = "InWorldContextObject"))
	static ACppTower* SpawnTower(UObject* InWorldContextObject,
		TSubclassOf<ACppTower> InClass,
		const FTransform& InTransform,
		const FCppTowerSpawnParameters& InSpawnParameters);

	UFUNCTION(BlueprintCallable, Category = "Spawn", Meta = (WorldContext = "InWorldContextObject"))
	static ACppProjectile* SpawnProjectile(UObject* InWorldContextObject,
		TSubclassOf<ACppProjectile> InClass,
		const FTransform& InTransform,
		const FCppProjectileSpawnParameters& InSpawnParameters);
};
