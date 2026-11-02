#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"

#include "CppCameraPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UFloatingPawnMovement;

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "BloodlustAwakening")
class BLOODLUSTAWAKENING_API ACppCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ACppCameraPawn();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UFloatingPawnMovement> MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MinZoom = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MaxZoom = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ZoomStep = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float PanSpeed = 1.5f;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ZoomInterpSpeed = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float PanInterpSpeed = 10.f;

private:
	float DesiredZoom = 2000.f;
	FVector2D RawPanInput = FVector2D::ZeroVector;
	FVector2D SmoothedPanInput = FVector2D::ZeroVector;

protected:
	void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Camera")
	float GetZoom() const;

	UFUNCTION(BlueprintCallable, Category = "Camera")
	float GetDesiredZoom() const;

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void AddPanInput(const FVector2D& InInput);

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void AddZoom(float InDelta);
};