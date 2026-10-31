#include "BloodlustAwakening/Core/Stats/CppStats.h"

FName FCppStatsDatabaseRow::GetArchetypeRowName(const UClass* InClass)
{
	if (!InClass)
	{
		return NAME_None;
	}
	FString NameString = InClass->GetFName().ToString();
	if (NameString.StartsWith(TEXT("BP_")))
	{
		NameString.RightChopInline(3);
	}
	if (NameString.EndsWith(TEXT("_C")))
	{
		NameString.LeftChopInline(2);
	}
	return FName(*NameString);
}
