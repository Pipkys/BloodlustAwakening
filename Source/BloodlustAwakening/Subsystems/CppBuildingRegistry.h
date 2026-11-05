#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Interface/CppGridPlaceable.h"

#include "CppBuildingRegistry.generated.h"

class UCppHealthComponent;

USTRUCT(BlueprintType)
struct FBuildingArray
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<TScriptInterface<ICppGridPlaceable>> Data;
};

UCLASS(BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppBuildingRegistry : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TMap<FTopLevelAssetPath, FBuildingArray> Buildings;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Registry", Meta = (WorldContext = "InWorldContextObject"))
	static TArray<TScriptInterface<ICppGridPlaceable>>& GetAllBuildingsOfClass(UObject* InWorldContextObject, TSubclassOf<AActor> InClass);

	UFUNCTION(BlueprintCallable, Category = "Registry")
	static void RegisterBuilding(TScriptInterface<ICppGridPlaceable> InBuilding, const TArray<ACppCell*>& InOccupiedCells);

	UFUNCTION(BlueprintCallable, Category = "Registry")
	static void UnregisterBuilding(TScriptInterface<ICppGridPlaceable> InBuilding);

	UFUNCTION(BlueprintCallable, Category = "Registry")
	static TScriptInterface<ICppGridPlaceable> GetBuildingByOccupiedCells(const TArray<ACppCell*>& InOccupiedCells);

	UFUNCTION(BlueprintCallable, Category = "Registry", Meta = (WorldContext = "InWorldContextObject"))
	static TScriptInterface<ICppGridPlaceable> GetBuildingByOccupiedCellCoords(UObject* InWorldContextObject, const TArray<FIntPoint>& InOccupiedCells);

private:
	static FTopLevelAssetPath GetTypeId(const UClass* InClass);

	UFUNCTION()
	void UnregisterOnDestroyed(AActor* InActor);

	UFUNCTION()
	void UnregisterOnDie(UCppHealthComponent* InHealthComponent);
};
