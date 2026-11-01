#include "BloodlustAwakening/GameMode/CppBloodlustAwakeningGameplayStatics.h"

#include "Kismet/KismetSystemLibrary.h"

bool UCppBloodlustAwakeningGameplayStatics::IsActorInRange(AActor* InActor,
	float InRange,
	const FVector& InLocation)
{
	return FVector::DistSquared(InLocation, InActor->GetActorLocation()) <= InRange * InRange;
}

AActor* UCppBloodlustAwakeningGameplayStatics::FindActorByPredicate(UObject* InWorldContextObject,
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

TArray<AActor*> UCppBloodlustAwakeningGameplayStatics::FindActors(UObject* InWorldContextObject,
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

TArray<AActor*> UCppBloodlustAwakeningGameplayStatics::FindActorsByPredicate(UObject* InWorldContextObject,
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

AActor* UCppBloodlustAwakeningGameplayStatics::FindNearestActor(const TArray<AActor*>& InActors,
	float InRange,
	const FVector& InLocation)
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

AActor* UCppBloodlustAwakeningGameplayStatics::FindNearestActorByPredicate(const TArray<AActor*>& InActors,
	float InRange,
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

AActor* UCppBloodlustAwakeningGameplayStatics::FindNearestActorByPredicate(const TArray<AActor*>& InActors,
	float InRange,
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

ACppUnit* UCppBloodlustAwakeningGameplayStatics::SpawnUnit(UObject* InWorldContextObject,
	TSubclassOf<ACppUnit> InClass,
	const FTransform& InTransform,
	const FCppUnitSpawnParameters& InSpawnParameters)
{
	ACppUnit* Unit = (ACppUnit*)InWorldContextObject->GetWorld()->SpawnActor(InClass,
		&InTransform,
		InSpawnParameters.ConvertToEngineType());
	if (Unit)
	{
		Unit->Setup(InSpawnParameters);
	}
	return Unit;
}

ACppDecoration* UCppBloodlustAwakeningGameplayStatics::SpawnDecoration(UObject* InWorldContextObject,
	TSubclassOf<ACppDecoration> InClass,
	const FTransform& InTransform,
	const FCppActorSpawnParameters& InSpawnParameters)
{
	ACppDecoration* Decoration = (ACppDecoration*)InWorldContextObject->GetWorld()->SpawnActor(InClass,
		&InTransform,
		InSpawnParameters.ConvertToEngineType());
	return Decoration;
}

ACppSpawner* UCppBloodlustAwakeningGameplayStatics::SpawnSpawner(UObject* InWorldContextObject,
	TSubclassOf<ACppSpawner> InClass,
	const FTransform& InTransform,
	const FCppActorSpawnParameters& InSpawnParameters)
{
	ACppSpawner* Spawner = (ACppSpawner*)InWorldContextObject->GetWorld()->SpawnActor(InClass,
		&InTransform,
		InSpawnParameters.ConvertToEngineType());
	return Spawner;
}

ACppTower* UCppBloodlustAwakeningGameplayStatics::SpawnTower(UObject* InWorldContextObject,
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

ACppProjectile* UCppBloodlustAwakeningGameplayStatics::SpawnProjectile(UObject* InWorldContextObject,
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
