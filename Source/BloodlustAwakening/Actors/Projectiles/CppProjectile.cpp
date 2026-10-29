#include "BloodlustAwakening/Actors/Projectiles/CppProjectile.h"

#include "BloodlustAwakening/Actors/Interface/CppEntity.h"
#include "BloodlustAwakening/Components/AttackComponent/CppAttackComponentStats.h"
#include "BloodlustAwakening/Components/AttackComponent/Ranged/CppRangedAttackComponentStats.h"
#include "BloodlustAwakening/Core/Physics/CppCollisionChannels.h"

#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

ACppProjectile::ACppProjectile()
{
	OverlapComponent = CreateDefaultSubobject<USphereComponent>("OverlapComponent");
	OverlapComponent->SetupAttachment(RootComponent);
	OverlapComponent->SetCollisionObjectType(ECC_Projectile);
	OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComponent->SetCollisionResponseToChannel(ECC_Building, ECR_Overlap);
	OverlapComponent->SetCollisionResponseToChannel(ECC_Unit, ECR_Overlap);
	OverlapComponent->SetCanEverAffectNavigation(false);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
}

void ACppProjectile::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);

	OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ACppProjectile::OnBeginOverlap);

	UNiagaraFunctionLibrary::SpawnSystemAttached(
		ShotEffect,
		OverlapComponent,
		"ShotEffect",
		FVector::Zero(),
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true);
}

void ACppProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACppProjectile::Setup(const FCppProjectileSpawnParameters& InSpawnParameters)
{
	TScriptInterface<ICppEntity> OwnerInterface = Owner;
	FCppStats* LoadedStats = OwnerInterface->GetStats(ECppStatsType::RangedAttack);

	if (!LoadedStats)
	{
		UE_LOG(LogTemp, Error, TEXT("ACppProjectile::Setup: Failed to get ranged attack stats from '%s'"), OwnerInterface.GetObject()->GetName().GetCharArray().GetData());
		return;
	}

	FCppRangedAttackComponentStats* Stats = (FCppRangedAttackComponentStats*)LoadedStats;
	OwnerRangedAttackStats = Stats;
	MovementComponent->bRotationFollowsVelocity = Stats->bProjectileRotationFollowsVelocity;
	MovementComponent->ProjectileGravityScale = Stats->ProjectileGravityScale;
	MovementComponent->bIsHomingProjectile = Stats->bIsHomingProjectile;
	MovementComponent->HomingAccelerationMagnitude = Stats->ProjectileHomingAccelerationMagnitude;
	MovementComponent->MaxSpeed = Stats->ProjectileMaxSpeed;
	MovementComponent->InitialSpeed = Stats->ProjectileInitialSpeed;
	OverlapComponent->SetSphereRadius(Stats->ProjectileOverlapRadius);
}

void ACppProjectile::LockOnTarget(AActor* InTarget)
{
	CurrentTarget = InTarget;
	MovementComponent->HomingTargetComponent = CurrentTarget->GetRootComponent();

	if (!PrimaryActorTick.bCanEverTick)
	{
		PrimaryActorTick.bCanEverTick = true;
		SetActorTickEnabled(true);
	}
}

void ACppProjectile::OnBeginOverlap(UPrimitiveComponent* InOverlappedComponent,
	AActor* InOtherActor,
	UPrimitiveComponent* InOtherComponent,
	int32 InOtherBodyIndex,
	bool bInFromSweep,
	const FHitResult& InSweepResult)
{
	if (!IsValid(InOtherActor))
	{
		return;
	}

	if (InOtherActor == this)
	{
		return;
	}

	if (InOtherActor != CurrentTarget)
	{
		return;
	}

	FCppRangedAttackComponentStats* Stats = (FCppRangedAttackComponentStats*)OwnerRangedAttackStats;
	UGameplayStatics::ApplyDamage(InOtherActor, Stats->Damage, GetInstigatorController(), this, Stats->DamageType);
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
