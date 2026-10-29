#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "Runtime/NavigationSystem/Public/NavModifierComponent.h"

#include "BloodlustAwakening/Actors/Interface/CppGridPlaceable.h"
#include "BloodlustAwakening/Level/Grid/CppCell.h"

#include "CppSpawner.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API ACppSpawner : public AActor, public ICppGridPlaceable
{
	GENERATED_BODY()

public:
	ACppSpawner();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Components")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Components")
	TObjectPtr<UNavModifierComponent> NavModifierComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|GridData")
	TArray<ACppCell*> OccupiedCells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|GridData")
	int32 SizeInCells;

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

public:
	int32 GetSizeInCells() const;
	TArray<ACppCell*>& GetOccupiedCells();
};
