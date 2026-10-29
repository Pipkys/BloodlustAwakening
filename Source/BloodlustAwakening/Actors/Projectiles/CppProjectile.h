#pragma once

#include "CoreMinimal.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraSystem.h"

#include "BloodlustAwakening/Actors/Projectiles/CppProjectileSpawnParameters.h"
#include "BloodlustAwakening/Core/Stats/CppStats.h"

#include "CppProjectile.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API ACppProjectile : public AActor
{
	GENERATED_BODY()

public:
	ACppProjectile();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Components")
	TObjectPtr<USphereComponent> OverlapComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Components")
	TObjectPtr<UProjectileMovementComponent> MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Effects")
	TObjectPtr<UNiagaraSystem> ShotEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Effects")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Target")
	TObjectPtr<AActor> CurrentTarget;

private:
	FCppStats* OwnerRangedAttackStats;

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	
public:
	void Setup(const FCppProjectileSpawnParameters& InSpawnParameters);

protected:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* InOverlappedComponent,
		AActor* InOtherActor,
		UPrimitiveComponent* InOtherComponent,
		int32 InOtherBodyIndex,
		bool bInFromSweep, const
		FHitResult& InSweepResult);

public:
	UFUNCTION(BlueprintCallable, Category = "Projectile|Target")
	void LockOnTarget(AActor* InTarget);

	UFUNCTION(BlueprintCallable, Category = "Projectile|LifeTime")
	void Explode();
};
