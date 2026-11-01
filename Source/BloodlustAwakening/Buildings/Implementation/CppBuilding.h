#pragma once

#include "CoreMinimal.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Runtime/NavigationSystem/Public/NavModifierComponent.h"

#include "BloodlustAwakening/Buildings/Implementation/CppBuildingSpawnParameters.h"
#include "BloodlustAwakening/Interface/CppEntity.h"
#include "BloodlustAwakening/Interface/CppGridPlaceable.h"
#include "BloodlustAwakening/Components/StatsComponent/CppStatsComponent.h"
#include "BloodlustAwakening/Components/TeamComponent/CppTeamComponent.h"
#include "BloodlustAwakening/Core/Stats/CppStats.h"
#include "BloodlustAwakening/Core/Team/CppTeam.h"
#include "BloodlustAwakening/Level/Grid/CppCell.h"
#include "BloodlustAwakening/Subsystems/CppStatsSubsystem.h"
#include "BloodlustAwakening/Subsystems/CppTeamSubsystem.h"

#include "CppBuilding.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API ACppBuilding : public APawn, public ICppEntity, public ICppGridPlaceable
{
	GENERATED_BODY()

public:
	ACppBuilding();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Components")
	TObjectPtr<UCppTeamComponent> TeamComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Components")
	TObjectPtr<UCppStatsComponent> StatsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Components")
	TObjectPtr<UNavModifierComponent> NavModifierComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|GridData")
	TArray<ACppCell*> OccupiedCells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|GridData")
	int32 SizeInCells;

protected:
	UPROPERTY()
	TObjectPtr<UCppTeamSubsystem> TeamSubsystem;

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

public:
	virtual void Setup(const FCppBuildingSpawnParameters& InSpawnParameters);

public:
	UStaticMeshComponent* GetStaticMeshComponent() override;
	UCppTeamComponent* GetTeamComponent() override;
	UCppStatsComponent* GetStatsComponent() override;
	UCppTeam* GetTeam() const override;
	bool IsHostileTo(TScriptInterface<ICppEntity> InOther) const override;
	
protected:
	void LoadStats() override;

public:
	float TakeDamage(float InBaseDamage,
		struct FDamageEvent const& InDamageEvent,
		class AController* InEventInstigator,
		AActor* InDamageCauser) override;

protected:
	void OnDie();

public:
	int32 GetSizeInCells() const;
	TArray<ACppCell*>& GetOccupiedCells();
};
