#include "BloodlustAwakening/Subsystems/CppBuildingRegistry.h"

#include "BloodlustAwakening/Components/HealthComponent/CppHealthComponent.h"
#include "BloodlustAwakening/Interface/CppDamageable.h"

TArray<TScriptInterface<ICppGridPlaceable>>& UCppBuildingRegistry::GetAllBuildingsOfClass(UObject* InWorldContextObject, TSubclassOf<AActor> InClass)
{
	if (!InClass)
	{
		UE_LOG(LogTemp,
			Fatal,
			TEXT("UCppBuildingRegistry::GetAllBuildingsOfClass: passed incorrect class"));
	}

	UCppBuildingRegistry* Instance = InWorldContextObject->GetWorld()->GetSubsystem<UCppBuildingRegistry>();
	const FTopLevelAssetPath& TypeId = GetTypeId(InClass);
	if (FBuildingArray* Found = Instance->Buildings.Find(TypeId))
	{
		return Found->Data;
	}

	UE_LOG(LogTemp,
		Fatal,
		TEXT("UCppBuildingRegistry::GetAllBuildingsOfClass: passed unknown type '%s'"),
		*TypeId.ToString());
	return *(TArray<TScriptInterface<ICppGridPlaceable>>*)nullptr;
}

void UCppBuildingRegistry::RegisterBuilding(TScriptInterface<ICppGridPlaceable> InBuilding, const TArray<ACppCell*>& InOccupiedCells)
{
	UCppBuildingRegistry* Instance = InBuilding.GetObject()->GetWorld()->GetSubsystem<UCppBuildingRegistry>();
	TMap<FTopLevelAssetPath, FBuildingArray>& Buildings = Instance->Buildings;
	FTopLevelAssetPath TypeId = GetTypeId(InBuilding.GetObject()->GetClass());
	if (FBuildingArray* FoundTypedBuildings = Buildings.Find(TypeId))
	{
		TArray<TScriptInterface<ICppGridPlaceable>>& TypedBuildings = FoundTypedBuildings->Data;
		if (TypedBuildings.FindByPredicate([InBuilding](TScriptInterface<ICppGridPlaceable>& Building)
		{
			return Building == InBuilding;
		}))
		{
			UE_LOG(LogTemp,
				Error,
				TEXT("UCppBuildingRegistry::RegisterBuilding: attempting to register already registred building '%s'"),
				*GetNameSafe(InBuilding.GetObject()));
			return;
		}
		TypedBuildings.Add(InBuilding);
	}
	else
	{
		Buildings.Emplace(std::move(TypeId), {}).Data.Add(InBuilding);
	}

	InBuilding->GetOccupiedCells() = InOccupiedCells;
	if (ICppDamageable* Damageable = Cast<ICppDamageable>(InBuilding.GetObject()))
	{
		Damageable->SubscribeToOnDieEvent(Instance, TEXT("UnregisterOnDie"));
	}
	else
	{
		AActor* Actor = (AActor*)InBuilding.GetObject();
		FScriptDelegate Delegate;
		Delegate.BindUFunction(Instance, TEXT("UnregisterOnDestroyed"));
		Actor->OnDestroyed.Add(Delegate);
	}
}

void UCppBuildingRegistry::UnregisterBuilding(TScriptInterface<ICppGridPlaceable> InBuilding)
{
	UCppBuildingRegistry* Instance = InBuilding.GetObject()->GetWorld()->GetSubsystem<UCppBuildingRegistry>();
	TMap<FTopLevelAssetPath, FBuildingArray>& Buildings = Instance->Buildings;
	const FTopLevelAssetPath& TypeId = GetTypeId(InBuilding.GetObject()->GetClass());
	if (FBuildingArray* FoundTypedBuildings = Buildings.Find(TypeId))
	{
		TArray<TScriptInterface<ICppGridPlaceable>>& TypedBuildings = FoundTypedBuildings->Data;
		int32 Index = TypedBuildings.Find(InBuilding);
		if (Index != INDEX_NONE)
		{
			TypedBuildings.RemoveAt(Index);
			return;
		}
	}

	UE_LOG(LogTemp,
		Error,
		TEXT("UCppBuildingRegistry::UnregisterBuilding: passed not registered building '%s'"),
		*GetNameSafe(InBuilding.GetObject()));
}

TScriptInterface<ICppGridPlaceable> UCppBuildingRegistry::GetBuildingByOccupiedCells(const TArray<ACppCell*>& InOccupiedCells)
{
	TArray<FIntPoint> CoordsArray;
	CoordsArray.Reserve(InOccupiedCells.Num());
	for (ACppCell* Cell : InOccupiedCells)
	{
		CoordsArray.Add(Cell->Coords);
	}
	return GetBuildingByOccupiedCellCoords(InOccupiedCells.Last(), CoordsArray);
}

TScriptInterface<ICppGridPlaceable> UCppBuildingRegistry::GetBuildingByOccupiedCellCoords(UObject* InWorldContextObject, const TArray<FIntPoint>& InOccupiedCells)
{
	UCppBuildingRegistry* Instance = InWorldContextObject->GetWorld()->GetSubsystem<UCppBuildingRegistry>();
	TMap<FTopLevelAssetPath, FBuildingArray>& Buildings = Instance->Buildings;
	for (TPair<FTopLevelAssetPath, FBuildingArray>& TypedBuildings : Buildings)
	{
		if (TScriptInterface<ICppGridPlaceable>* Building
			= TypedBuildings.Value.Data.FindByPredicate([InOccupiedCells](TScriptInterface<ICppGridPlaceable>& Building)
		{
			TScriptInterface<ICppGridPlaceable>& ActualBuilding = Building;
			if (ActualBuilding->GetOccupiedCells().Num() != InOccupiedCells.Num())
			{
				return false;
			}
			for (int32 Index = 0; Index < ActualBuilding->GetOccupiedCells().Num(); ++Index)
			{
				if (ActualBuilding->GetOccupiedCells()[Index]->Coords != InOccupiedCells[Index])
				{
					return false;
				}
			}
			return true;
		}))
		{
			return *Building;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("UCppBuildingRegistry::GetOccupiedCellsByBuilding: building not found by passed cell coords"));
	return nullptr;
}

FTopLevelAssetPath UCppBuildingRegistry::GetTypeId(const UClass* InClass)
{
	const UClass* Type = InClass;
	while (Type && Cast<UBlueprintGeneratedClass>(Type))
	{
		const UClass* Super = Cast<UClass>(Type->GetSuperStruct());
		if (!Super || Super == Type)
		{
			break;
		}
		Type = Super;
	}
	return Type ? Type->GetClassPathName() : FTopLevelAssetPath();
}

void UCppBuildingRegistry::UnregisterOnDestroyed(AActor* InActor)
{
	UCppBuildingRegistry::UnregisterBuilding(InActor);
}

void UCppBuildingRegistry::UnregisterOnDie(UCppHealthComponent* InHealthComponent)
{
	UCppBuildingRegistry::UnregisterBuilding(InHealthComponent->GetOwner());
}
