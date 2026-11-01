#include "BloodlustAwakening/Projectiles/CppProjectile.h"

#include "BloodlustAwakening/Core/Physics/CppCollisionChannels.h"

#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

ACppProjectile::ACppProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	OverlapComponent = CreateDefaultSubobject<USphereComponent>("OverlapComponent");
	OverlapComponent->SetupAttachment(RootComponent);
	OverlapComponent->SetCollisionObjectType(ECC_Projectile);
	OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	OverlapComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComponent->SetCollisionResponseToChannel(ECC_Building, ECR_Overlap);
	OverlapComponent->SetCollisionResponseToChannel(ECC_Unit, ECR_Overlap);
	OverlapComponent->SetCanEverAffectNavigation(false);
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
}

void ACppProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ACppProjectile::OnBeginOverlap);
}

void ACppProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsValid(CurrentTarget))
	{
		Explode();
	}
}

void ACppProjectile::Setup(const FCppProjectileSpawnParameters& InSpawnParameters)
{
	DamageType = InSpawnParameters.DamageType;
	ShotEffect = InSpawnParameters.ShotEffect;
	ImpactEffect = InSpawnParameters.ImpactEffect;
	Damage = InSpawnParameters.Damage;
	MovementComponent->bRotationFollowsVelocity = InSpawnParameters.bRotationFollowsVelocity;
	MovementComponent->ProjectileGravityScale = InSpawnParameters.GravityScale;
	MovementComponent->bIsHomingProjectile = InSpawnParameters.bIsHoming;
	MovementComponent->HomingAccelerationMagnitude = InSpawnParameters.HomingAccelerationMagnitude;
	MovementComponent->InitialSpeed = InSpawnParameters.InitialSpeed;
	MovementComponent->MaxSpeed = InSpawnParameters.MaxSpeed;
	OverlapComponent->SetSphereRadius(InSpawnParameters.OverlapRadius);
}

void ACppProjectile::LockOnTarget(AActor* InTarget)
{
	if (!IsValid(InTarget))
	{
		UE_LOG(LogTemp, Warning, TEXT("ACppProjectile::LockOnTarget: invalid target"));
		return;
	}
	CurrentTarget = InTarget;
	MovementComponent->HomingTargetComponent = CurrentTarget->GetRootComponent();
	if (!IsActorTickEnabled())
	{
		SetActorTickEnabled(true);
	}
	UNiagaraFunctionLibrary::SpawnSystemAttached(
		ShotEffect,
		OverlapComponent,
		"ShotEffect",
		FVector::Zero(),
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true);
}

void ACppProjectile::OnBeginOverlap(UPrimitiveComponent* InOverlappedComponent,
	AActor* InOtherActor,
	UPrimitiveComponent* InOtherComponent,
	int32 InOtherBodyIndex,
	bool bInFromSweep,
	const FHitResult& InSweepResult)
{
	if (!IsValid(InOtherActor) || InOtherActor == this)
	{
		return;
	}
	if (!IsValid(CurrentTarget) || InOtherActor != CurrentTarget)
	{
		return;
	}
	UGameplayStatics::ApplyDamage(InOtherActor, Damage, GetInstigatorController(), this, DamageType);
	Explode();
}

void ACppProjectile::Explode()
{
	if (IsValid(ImpactEffect))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactEffect, GetActorLocation());
	}
	Destroy();
}
