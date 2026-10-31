#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"

#include "CppStats.generated.h"

USTRUCT(BlueprintType)
struct BLOODLUSTAWAKENING_API FCppStatsDatabaseRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	static FName GetArchetypeRowName(const UClass* InClass);
};

using FCppStats = FCppStatsDatabaseRow;

UENUM(BlueprintType)
enum class ECppStatsType : uint8
{
	Health = 0 UMETA(DisplayName = "Health"),
	Attack UMETA(DisplayName = "Attack"),
	Unit UMETA(DisplayName = "Unit"),
	Building UMETA(DisplayName = "Building"),
	Tower UMETA(DisplayName = "Tower")
};

// ============================================================================
//  Upgrade system
//
//  ONE upgrade DataTable for the whole game: a row is an upgrade track bound
//  to an archetype, each level carries blood cost + stat modifiers. Levels
//  are applied additively/multiplicatively on top of the archetype's base
//  values by UCppStatsComponent::GetStatValue.
// ============================================================================

//UENUM(BlueprintType)
//enum class ECppStatModifierType : uint8
//{
//	Add UMETA(DisplayName = "Add"),
//	Multiply UMETA(DisplayName = "Multiply")
//};
//
//USTRUCT(BlueprintType)
//struct BLOODLUSTAWAKENING_API FCppStatModifier
//{
//	GENERATED_BODY()
//
//public:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
//	FName StatId;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
//	ECppStatModifierType Type = ECppStatModifierType::Add;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
//	float Value = 0.0f;
//};
//
//USTRUCT(BlueprintType)
//struct BLOODLUSTAWAKENING_API FCppStatsUpgradeLevel
//{
//	GENERATED_BODY()
//
//public:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
//	int32 Level = 1;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
//	float BloodCost = 0.0f;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
//	TArray<FCppStatModifier> Modifiers;
//};
//
//USTRUCT(BlueprintType)
//struct BLOODLUSTAWAKENING_API FCppStatsUpgradeRow : public FCppStatsDatabaseRow
//{
//	GENERATED_BODY()
//
//public:
//	// Archetype this track upgrades (matches FCppEntityStatsRow row name).
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
//	FName ArchetypeId;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
//	TArray<FCppStatsUpgradeLevel> Levels;
//
//public:
//	int32 GetMaxLevel() const;
//	const FCppStatsUpgradeLevel* FindLevel(int32 InLevel) const;
//};
//
//// ============================================================================
////  Stat ids used by modifiers
//// ============================================================================
//
//namespace CppStatIds
//{
//	inline const FName MaxHealth(TEXT("MaxHealth"));
//	inline const FName DetectionRange(TEXT("DetectionRange"));
//	inline const FName MovementSpeed(TEXT("MovementSpeed"));
//	inline const FName Damage(TEXT("Damage"));
//	inline const FName AttackSpeed(TEXT("AttackSpeed"));
//	inline const FName AttackRange(TEXT("AttackRange"));
//}

template<ECppStatsType>
struct TCppStatsTypeTraits;
