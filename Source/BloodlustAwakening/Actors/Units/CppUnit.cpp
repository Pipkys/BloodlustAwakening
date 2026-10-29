#include "BloodlustAwakening/Actors/Units/CppUnit.h"

#include "BloodlustAwakening/Actors/AI/CppAIController.h"
#include "BloodlustAwakening/Actors/Units/CppUnitStats.h"
#include "BloodlustAwakening/Core/Physics/CppCollisionChannels.h"

ACppUnit::ACppUnit()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ACppAIController::StaticClass();

	StaticMeshComponent =CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionObjectType(ECC_Unit);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	StaticMeshComponent->SetCanEverAffectNavigation(false);

	TeamComponent = CreateDefaultSubobject<UCppTeamComponent>("TeamComponent");
	StatsComponent = CreateDefaultSubobject<UCppStatsComponent>("StatsComponent");
	HealthComponent = CreateDefaultSubobject<UCppHealthComponent>("HealthComponent");

	UCapsuleComponent* BodyComponent = GetCapsuleComponent();
	BodyComponent->SetCollisionObjectType(ECC_Unit);
	BodyComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BodyComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BodyComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	BodyComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	BodyComponent->SetCollisionResponseToChannel(ECC_Building, ECR_Block);
	BodyComponent->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	BodyComponent->SetCollisionResponseToChannel(ECC_Level, ECR_Block);
	BodyComponent->SetCanEverAffectNavigation(false);

	USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
	SkeletalMeshComponent->SetCollisionObjectType(ECC_Unit);
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SkeletalMeshComponent->SetCanEverAffectNavigation(false);

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->UpdatedComponent = GetCapsuleComponent();
	MovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);

	MovementComponent->MaxWalkSpeed = 600.0f;
	MovementComponent->MaxAcceleration = 2048.0f;
	MovementComponent->BrakingDecelerationWalking = 8192.0f;
	MovementComponent->BrakingFrictionFactor = 1.0f;
	MovementComponent->BrakingFriction = 1.0f;
	
	MovementComponent->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->bUseControllerDesiredRotation = true;
	
	MovementComponent->JumpZVelocity = 600.0f;
	MovementComponent->AirControl = 0.05f;

	AttackPointComponent = CreateDefaultSubobject<USphereComponent>("AttackPointComponent");
	AttackPointComponent->SetupAttachment(RootComponent);
	AttackPointComponent->SetCollisionObjectType(ECC_Trigger);
	AttackPointComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackPointComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackPointComponent->SetCanEverAffectNavigation(false);
}

void ACppUnit::BeginPlay()
{
	Super::BeginPlay();
	
	TeamSubsystem = GetWorld()->GetSubsystem<UCppTeamSubsystem>();
	StatsSubsystem = GetWorld()->GetSubsystem<UCppStatsSubsystem>();
}

void ACppUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACppUnit::Setup(const FCppUnitSpawnParameters& InSpawnParameters)
{
	TeamComponent->Team = InSpawnParameters.Team;

	AttackPointComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackPointComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackPointComponent->SetCanEverAffectNavigation(false);

	FCppUnitStats* UnitStats = (FCppUnitStats*)StatsSubsystem->LoadStats(this, ECppStatsType::Unit);
	if (UnitStats)
	{
		DetectionRange = UnitStats->DetectionRange;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACppUnit::Setup: Failed to load Unit stats"));
	}

	HealthComponent->SetupComponent(this, InSpawnParameters);
	AttackComponent->SetupComponent(this, InSpawnParameters);
}

UStaticMeshComponent* ACppUnit::GetStaticMeshComponent()
{
	return StaticMeshComponent;
}

UCppTeamComponent* ACppUnit::GetTeamComponent()
{
	return TeamComponent;
}

UCppStatsComponent* ACppUnit::GetStatsComponent()
{
	return StatsComponent;
}

UCppTeam* ACppUnit::GetTeam() const
{
	return TeamComponent->Team;
}

FCppStats* ACppUnit::GetStats(ECppStatsType InStatsType)
{
	return StatsComponent->GetStats(GetClass(), InStatsType);
}

void ACppUnit::CacheStats(const FCppStatsView& InStatsView)
{
	StatsComponent->LoadedStats.Add(InStatsView);
}

bool ACppUnit::IsHostileTo(TScriptInterface<ICppEntity> InOther) const
{
	return TeamSubsystem->IsHostile((UObject*)this, InOther);
}

float ACppUnit::TakeDamage(float InBaseDamage,
	struct FDamageEvent const& InDamageEvent,
	class AController* InEventInstigator,
	AActor* InDamageCauser)
{
	HealthComponent->OnTakeDamage(InBaseDamage, InDamageEvent, InEventInstigator, InDamageCauser);
	float Diff = HealthComponent->CurrentHealth - InBaseDamage;
	HealthComponent->SetCurrentHealth(FMath::Clamp(Diff, 0.f, HealthComponent->MaxHealth));
	if (Diff <= 0.f)
	{
		Die();
	}
	return InBaseDamage;
}

UCppHealthComponent* ACppUnit::GetHealthComponent()
{
	return HealthComponent;
}

float ACppUnit::GetMaxHealth() const
{
	return HealthComponent->GetMaxHealth();
}

void ACppUnit::SetMaxHealth(float InMaxHealth)
{
	HealthComponent->SetMaxHealth(InMaxHealth);
}

float ACppUnit::GetCurrentHealth() const
{
	return HealthComponent->GetCurrentHealth();
}

void ACppUnit::SetCurrentHealth(float InCurrentHealth)
{
	HealthComponent->SetCurrentHealth(InCurrentHealth);
}

bool ACppUnit::IsAlive() const
{
	return HealthComponent->IsAlive();
}

bool ACppUnit::IsDead() const
{
	return HealthComponent->IsDead();
}

void ACppUnit::SubscribeToOnTakeDamageEvent(UObject* InObject, const FName& InFunctionName)
{
	HealthComponent->SubscribeToOnTakeDamageEvent(InObject, InFunctionName);
}

void ACppUnit::SubscribeToOnDieEvent(UObject* InObject, const FName& InFunctionName)
{
	HealthComponent->SubscribeToOnDieEvent(InObject, InFunctionName);
}

void ACppUnit::Die()
{
	HealthComponent->OnDie();
	Destroy();
}

UCppAttackComponent* ACppUnit::GetAttackComponent()
{
	return AttackComponent;
}

USphereComponent* ACppUnit::GetAttackPointComponent()
{
	return AttackPointComponent;
}

AActor* ACppUnit::GetCurrentTarget() const
{
	return CurrentTarget;
}

void ACppUnit::LockOnTarget(AActor* InTarget)
{
	bLockedOnTarget = true;
	CurrentTarget = InTarget;
}

void ACppUnit::ReleaseTarget()
{
	bLockedOnTarget = false;
	CurrentTarget = nullptr;
}

bool ACppUnit::IsTargetInAttackRange(AActor* InTarget, const FTransform& InAttackPointTransform) const
{
	return AttackComponent->IsTargetInAttackRange(InTarget, InAttackPointTransform);
}

bool ACppUnit::IsCanAttack() const
{
	return AttackComponent->IsCanAttack();
}

bool ACppUnit::IsCanAttackTarget(AActor* InTarget, const FTransform& InAttackPointTransform) const
{
	return AttackComponent->IsCanAttackTarget(InTarget, InAttackPointTransform);
}
