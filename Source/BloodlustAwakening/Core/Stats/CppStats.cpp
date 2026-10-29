#include "BloodlustAwakening/Core/Stats/CppStats.h"

#include "BloodlustAwakening/Subsystems/CppStatsSubsystem.h"

namespace BloodlustAwakening::Private
{
	static FName ExtractNameFromBlueprintName(const FString& FullString)
	{
		int32 FirstUnderscore = FullString.Find(TEXT("_"));
		UE_LOG(LogTemp, Warning, TEXT("ExtractNameFromBlueprintName: FullString: %s"), *FullString);
		UE_LOG(LogTemp, Warning, TEXT("ExtractNameFromBlueprintName: First Underscore: %i"), FirstUnderscore);
		if (FirstUnderscore == INDEX_NONE)
		{
			return *FullString;
		}
		FString temp = FullString.Mid(FirstUnderscore + 1);
		UE_LOG(LogTemp, Warning, TEXT("ExtractNameFromBlueprintName: Mid of it: %s"), *temp);
		int32 SecondUnderscore = temp.Find(TEXT("_"));
		UE_LOG(LogTemp, Warning, TEXT("ExtractNameFromBlueprintName: Second Underscore: %i"), SecondUnderscore);
		if (SecondUnderscore == INDEX_NONE)
		{
			return *temp;
		}
		UE_LOG(LogTemp, Warning, TEXT("ExtractNameFromBlueprintName: Mid of second: %s"), *temp.Mid(0, SecondUnderscore));
		return *temp.Mid(0, SecondUnderscore);
	}

	static FName ExtractNameFromBlueprintName(const FName& FullString)
	{
		return ExtractNameFromBlueprintName(FullString.ToString());
	}
}

FCppStats* FCppStatsDatabaseRowView::Load(UClass* InClass, const UDataTable* InDatabase)
{
	return InDatabase->FindRow<FCppStats>(BloodlustAwakening::Private::ExtractNameFromBlueprintName(InClass->GetFName().ToString()),
		"FCppStatsDatabaseRowView::Load");
}

FCppStats* FCppStatsDatabaseRowView::Load(UObject* InObject, const UDataTable* InDatabase)
{
	return InDatabase->FindRow<FCppStats>(BloodlustAwakening::Private::ExtractNameFromBlueprintName(InObject->GetClass()->GetFName()),
		"FCppStatsDatabaseRowView::Load");
}

FCppStats* FCppStatsDatabaseRowView::Load() const
{
	return Database->FindRow<FCppStats>(BloodlustAwakening::Private::ExtractNameFromBlueprintName(ClassBoundedTo->GetFName()),
		"FCppStatsDatabaseRowView::Load");
}
