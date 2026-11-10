#include "BloodlustAwakening/Buildings/Implementation/CppGridBuilding.h"

#include "BloodlustAwakening/Core/Physics/CppCollisionChannels.h"

ACppGridBuilding::ACppGridBuilding()
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
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCanEverAffectNavigation(false);

	NavModifierComponent = CreateDefaultSubobject<UNavModifierComponent>("NavModifierComponent");
}

void ACppGridBuilding::EnableBoxBody(bool bAffectsNavigation)
{
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionResponseToChannel(ECC_Unit, ECollisionResponse::ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECC_Building, ECollisionResponse::ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECC_Projectile, ECollisionResponse::ECR_Overlap);
	BoxComponent->SetCanEverAffectNavigation(bAffectsNavigation);
}

void ACppGridBuilding::RestoreActorDefaults()
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	SetReplicates(false);
	SetRemoteRoleForBackwardsCompat(ROLE_None);
	SetReplicatingMovement(false);
	NetPriority = 1.0f;
	bCollideWhenPlacing = false;
	bGenerateOverlapEventsDuringLevelStreaming = false;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

void ACppGridBuilding::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	if (bStaticBuilding)
	{
		SetInstigator(nullptr);
	}
}

int32 ACppGridBuilding::GetSizeInCells() const
{
	return SizeInCells;
}

TArray<ACppCell*>& ACppGridBuilding::GetOccupiedCells()
{
	return OccupiedCells;
}
