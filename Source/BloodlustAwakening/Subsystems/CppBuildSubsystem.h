#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Buildings/Decorations/Implementation/CppDecoration.h"
#include "BloodlustAwakening/Buildings/Spawners/Implementation/CppSpawner.h"
#include "BloodlustAwakening/Buildings/Towers/Implementation/CppTower.h"

#include "CppBuildSubsystem.generated.h"

UCLASS(BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppBuildSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Build")
	static ACppSpawner* BuildSpawner(AActor* InOwner,
		const TArray<ACppCell*>& InOccupiedCells,
		TSubclassOf<ACppSpawner> InClass,
		const FTransform& InTransform);

	UFUNCTION(BlueprintCallable, Category = "Build")
	static ACppDecoration* BuildDecoration(AActor* InOwner,
		const TArray<ACppCell*>& InOccupiedCells,
		TSubclassOf<ACppDecoration> InClass,
		const FTransform& InTransform,
		bool bSmallEnough);

	UFUNCTION(BlueprintCallable, Category = "Build")
	static ACppTower* BuildTower(AActor* InOwner,
		const TArray<ACppCell*>& InOccupiedCells,
		TSubclassOf<ACppTower> InClass,
		const FTransform& InTransform,
		const FName& InTeamName);
};
