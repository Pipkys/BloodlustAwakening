#include "BloodlustAwakening/Actors/Decorations/CppDecoration.h"

#include "BloodlustAwakening/Core/Physics/CppCollisionChannels.h"

ACppDecoration::ACppDecoration()
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
	BoxComponent->SetCanEverAffectNavigation(true);

	NavModifierComponent = CreateDefaultSubobject<UNavModifierComponent>("NavModifierComponent");
}

void ACppDecoration::BeginPlay()
{
	Super::BeginPlay();
}

void ACppDecoration::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int32 ACppDecoration::GetSizeInCells() const
{
	return SizeInCells;
}

TArray<ACppCell*>& ACppDecoration::GetOccupiedCells()
{
	return OccupiedCells;
}
