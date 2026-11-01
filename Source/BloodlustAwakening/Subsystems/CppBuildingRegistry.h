#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Interface/CppGridPlaceable.h"

#include "CppBuildingRegistry.generated.h"

UCLASS(BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppBuildingRegistry : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<TScriptInterface<ICppGridPlaceable>> Buildings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<TScriptInterface<ICppGridPlaceable>> Decorations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<TScriptInterface<ICppGridPlaceable>> Spawners;

public:
	UFUNCTION(BlueprintCallable, Category = "Registry")
	static void RegisterBuilding(TScriptInterface<ICppGridPlaceable> InBuilding, const TArray<ACppCell*>& InOccupiedCells);

	UFUNCTION(BlueprintCallable, Category = "Registry")
	static void UnregisterBuilding(TScriptInterface<ICppGridPlaceable> InBuilding);

	UFUNCTION(BlueprintCallable, Category = "Registry")
	static void RegisterDecoration(TScriptInterface<ICppGridPlaceable> InDecoration, const TArray<ACppCell*>& InOccupiedCells);

	UFUNCTION(BlueprintCallable, Category = "Registry")
	static void UnregisterDecoration(TScriptInterface<ICppGridPlaceable> InDecoration);

	UFUNCTION(BlueprintCallable, Category = "Registry")
	static void RegisterSpawner(TScriptInterface<ICppGridPlaceable> InSpawner, const TArray<ACppCell*>& InOccupiedCells);

	UFUNCTION(BlueprintCallable, Category = "Registry")
	static void UnregisterSpawner(TScriptInterface<ICppGridPlaceable> InSpawner);

	UFUNCTION(BlueprintCallable, Category = "Registry")
	static TScriptInterface<ICppGridPlaceable> GetBuildingByOccupiedCells(const TArray<ACppCell*>& InOccupiedCells);

	UFUNCTION(BlueprintCallable, Category = "Registry", Meta = (WorldContext = "InWorldContextObject"))
	static TScriptInterface<ICppGridPlaceable> GetBuildingByOccupiedCellCoords(UObject* InWorldContextObject, 
		const TArray<FIntPoint>& InOccupiedCells);
};
