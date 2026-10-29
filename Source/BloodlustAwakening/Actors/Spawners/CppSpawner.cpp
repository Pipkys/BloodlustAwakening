#include "BloodlustAwakening/Actors/Spawners/CppSpawner.h"

#include "BloodlustAwakening/Core/Physics/CppCollisionChannels.h"

ACppSpawner::ACppSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionObjectType(ECC_Building);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	StaticMeshComponent->SetCanEverAffectNavigation(false);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionObjectType(ECC_Building);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Unit, ECollisionResponse::ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECC_Building, ECollisionResponse::ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECC_Projectile, ECollisionResponse::ECR_Overlap);
	BoxComponent->SetCanEverAffectNavigation(false);

	NavModifierComponent = CreateDefaultSubobject<UNavModifierComponent>("NavModifierComponent");
}

void ACppSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void ACppSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int32 ACppSpawner::GetSizeInCells() const
{
	return SizeInCells;
}

TArray<ACppCell*>& ACppSpawner::GetOccupiedCells()
{
	return OccupiedCells;
}
