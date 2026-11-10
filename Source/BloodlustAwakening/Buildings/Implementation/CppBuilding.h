#pragma once

#include "CoreMinimal.h"

#include "BloodlustAwakening/Buildings/Implementation/CppBuildingSpawnParameters.h"
#include "BloodlustAwakening/Buildings/Implementation/CppGridBuilding.h"
#include "BloodlustAwakening/Interface/CppEntity.h"
#include "BloodlustAwakening/Components/StatsComponent/CppStatsComponent.h"
#include "BloodlustAwakening/Components/TeamComponent/CppTeamComponent.h"
#include "BloodlustAwakening/Core/Stats/CppStats.h"
#include "BloodlustAwakening/Core/Team/CppTeam.h"
#include "BloodlustAwakening/Subsystems/CppTeamSubsystem.h"

#include "Components/CapsuleComponent.h"

#include "CppBuilding.generated.h"

class UCppWorldUIComponent;

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API ACppBuilding : public ACppGridBuilding, public ICppEntity
{
	GENERATED_BODY()

public:
	ACppBuilding();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Components")
	TObjectPtr<UCppTeamComponent> TeamComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Components")
	TObjectPtr<UCppStatsComponent> StatsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Components")
	TObjectPtr<UCppWorldUIComponent> WorldUIComponent;

protected:
	UPROPERTY()
	TObjectPtr<UCppTeamSubsystem> TeamSubsystem;

protected:
	void BeginPlay() override;

public:
	virtual void Setup(const FCppBuildingSpawnParameters& InSpawnParameters);

public:
	UStaticMeshComponent* GetStaticMeshComponent() override;
	UCppTeamComponent* GetTeamComponent() override;
	UCppStatsComponent* GetStatsComponent() override;
	bool IsHostileTo(TScriptInterface<ICppEntity> InOther) const override;

protected:
	void LoadStats() override;

public:
	float TakeDamage(float InBaseDamage,
		struct FDamageEvent const& InDamageEvent,
		class AController* InEventInstigator,
		AActor* InDamageCauser) override;

protected:
	void ForceDie();
};
