#include "BloodlustAwakening/GameMode/CppGameplayStatics.h"

#include "Kismet/KismetSystemLibrary.h"

bool UCppGameplayStatics::IsActorInRange(AActor* InActor,
	float InRange,
	const FVector& InLocation)
{
	return FVector::DistSquared(InLocation, InActor->GetActorLocation()) <= InRange * InRange;
}

AActor* UCppGameplayStatics::FindActorByPredicate(UObject* InWorldContextObject,
	float InRange,
	const TArray<TEnumAsByte<ECollisionChannel>>& InActorTypes,
	UClass* InActorClassFilter,
	const TArray<AActor*>& InActorsToIgnore,
	const FVector& InOverlapLocation,
	const FCppActorPredicate& InPredicate)
{
	TArray<AActor*> Actors = FindActors(InWorldContextObject, InRange, InActorTypes, InActorClassFilter, InActorsToIgnore, InOverlapLocation);
	for (AActor* Actor : Actors)
	{
		if (InPredicate.Execute(Actor))
		{
			return Actor;
		}
	}

	return nullptr;
}

TArray<AActor*> UCppGameplayStatics::FindActors(UObject* InWorldContextObject,
	float InRange,
	const TArray<TEnumAsByte<ECollisionChannel>>& InActorTypes,
	UClass* InActorClassFilter,
	const TArray<AActor*>& InActorsToIgnore,
	const FVector& InOverlapLocation)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Reserve(InActorTypes.Num());
	for (TEnumAsByte<ECollisionChannel> ActorType : InActorTypes)
	{
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ActorType.GetValue()));
	}
	TArray<AActor*> Result;
	UKismetSystemLibrary::SphereOverlapActors(InWorldContextObject,
		InOverlapLocation,
		InRange,
		ObjectTypes,
		InActorClassFilter,
		InActorsToIgnore,
		Result);
	return Result;
}

TArray<AActor*> UCppGameplayStatics::FindActorsByPredicate(UObject* InWorldContextObject,
	float InRange,
	const TArray<TEnumAsByte<ECollisionChannel>>& InActorTypes,
	UClass* InActorClassFilter,
	const TArray<AActor*>& InActorsToIgnore,
	const FVector& InOverlapLocation,
	const FCppActorPredicate& InPredicate)
{
	TArray<AActor*> Actors = FindActors(InWorldContextObject, InRange, InActorTypes, InActorClassFilter, InActorsToIgnore, InOverlapLocation);
	TArray<AActor*> Result;
	for (AActor* Actor : Actors)
	{
		if (InPredicate.Execute(Actor))
		{
			Result.Add(Actor);
		}
	}

	return Result;
}

AActor* UCppGameplayStatics::FindNearestActor(const TArray<AActor*>& InActors, const FVector& InLocation)
{
	AActor* NearestActor = nullptr;
	float MinDistanceSquared = TNumericLimits<float>::Max();
	for (AActor* Actor : InActors)
	{
		if (!IsValid(Actor))
		{
			continue;
		}
		const float DistanceSquared = FVector::DistSquared(Actor->GetActorLocation(), InLocation);
		if (DistanceSquared < MinDistanceSquared)
		{
			MinDistanceSquared = DistanceSquared;
			NearestActor = Actor;
		}
	}
	return NearestActor;
}

AActor* UCppGameplayStatics::FindNearestActorByPredicate(const TArray<AActor*>& InActors,
	const FVector& InLocation,
	const FCppActorPredicate& InPredicate)
{
	AActor* NearestActor = nullptr;
	float MinDistanceSquared = TNumericLimits<float>::Max();
	for (AActor* Actor : InActors)
	{
		if (!IsValid(Actor))
		{
			continue;
		}
		if (!InPredicate.Execute(Actor))
		{
			continue;
		}
		const float DistanceSquared = FVector::DistSquared(Actor->GetActorLocation(), InLocation);
		if (DistanceSquared < MinDistanceSquared)
		{
			MinDistanceSquared = DistanceSquared;
			NearestActor = Actor;
		}
	}
	return NearestActor;
}

AActor* UCppGameplayStatics::FindNearestActorByPredicate(const TArray<AActor*>& InActors,
	const FVector& InLocation,
	const TFunctionRef<bool(AActor*)>& InPredicate)
{
	AActor* NearestActor = nullptr;
	float MinDistanceSquared = TNumericLimits<float>::Max();
	for (AActor* Actor : InActors)
	{
		if (!IsValid(Actor))
		{
			continue;
		}
		if (!InPredicate(Actor))
		{
			continue;
		}
		const float DistanceSquared = FVector::DistSquared(Actor->GetActorLocation(), InLocation);
		if (DistanceSquared < MinDistanceSquared)
		{
			MinDistanceSquared = DistanceSquared;
			NearestActor = Actor;
		}
	}
	return NearestActor;
}

ACppUnit* UCppGameplayStatics::SpawnUnit(UObject* InWorldContextObject,
	TSubclassOf<ACppUnit> InClass,
	const FTransform& InTransform,
	const FCppUnitSpawnParameters& InSpawnParameters)
{
	FTransform AdjustedTransform;
	AdjustedTransform.SetLocation({ InTransform.GetLocation().X, InTransform.GetLocation().Y, InTransform.GetLocation().Z
		+ ((ACharacter*)InClass->GetDefaultObject())->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() });

	ACppUnit* Unit = (ACppUnit*)InWorldContextObject->GetWorld()->SpawnActor(InClass,
		&AdjustedTransform,
		InSpawnParameters.ConvertToEngineType());

	if (Unit)
	{
		Unit->Setup(InSpawnParameters);
	}

	return Unit;
}

ACppDecoration* UCppGameplayStatics::SpawnDecoration(UObject* InWorldContextObject,
	TSubclassOf<ACppDecoration> InClass,
	const FTransform& InTransform,
	const FCppActorSpawnParameters& InSpawnParameters)
{
	ACppDecoration* Decoration = (ACppDecoration*)InWorldContextObject->GetWorld()->SpawnActor(InClass,
		&InTransform,
		InSpawnParameters.ConvertToEngineType());
	return Decoration;
}

ACppSpawner* UCppGameplayStatics::SpawnSpawner(UObject* InWorldContextObject,
	TSubclassOf<ACppSpawner> InClass,
	const FTransform& InTransform,
	const FCppActorSpawnParameters& InSpawnParameters)
{
	ACppSpawner* Spawner = (ACppSpawner*)InWorldContextObject->GetWorld()->SpawnActor(InClass,
		&InTransform,
		InSpawnParameters.ConvertToEngineType());
	return Spawner;
}

ACppTower* UCppGameplayStatics::SpawnTower(UObject* InWorldContextObject,
	TSubclassOf<ACppTower> InClass,
	const FTransform& InTransform,
	const FCppTowerSpawnParameters& InSpawnParameters)
{
	ACppTower* Tower = (ACppTower*)InWorldContextObject->GetWorld()->SpawnActor(InClass,
		&InTransform,
		InSpawnParameters.ConvertToEngineType());
	if (Tower)
	{
		Tower->Setup(InSpawnParameters);
	}
	return Tower;
}

ACppProjectile* UCppGameplayStatics::SpawnProjectile(UObject* InWorldContextObject,
	TSubclassOf<ACppProjectile> InClass,
	const FTransform& InTransform,
	const FCppProjectileSpawnParameters& InSpawnParameters)
{
	ACppProjectile* Projectile = (ACppProjectile*)InWorldContextObject->GetWorld()->SpawnActor(InClass,
		&InTransform,
		InSpawnParameters.ConvertToEngineType());
	if (Projectile)
	{
		Projectile->Setup(InSpawnParameters);
	}
	return Projectile;
}
