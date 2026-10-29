#pragma once

#include "CoreMinimal.h"

#include "CppTeam.generated.h"

UENUM(BlueprintType)
enum class ECppTeamRelationship : uint8
{
	Friendly = 0 UMETA(DisplayName = "Friendly"),
	Neutral UMETA(DisplayName = "Neutral"),
	Hostile UMETA(DisplayName = "Hostile")
};

UCLASS(BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppTeam : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	FName Name;
};

UCLASS(BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API UCppTeamLink : public UObject
{
	GENERATED_BODY()

public:
	UCppTeam* FirstTeam;
	UCppTeam* SecondTeam;
	ECppTeamRelationship Relationship;
};
