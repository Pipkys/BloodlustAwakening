#include "BloodlustAwakening/Player/CppCameraPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"

ACppCameraPawn::ACppCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = DesiredZoom;
	SpringArmComponent->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritYaw = false;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->bUsePawnControlRotation = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent");
	MovementComponent->MaxSpeed = 2000.f;
	MovementComponent->Acceleration = 4000.f;
	MovementComponent->Deceleration = 4000.f;
	MovementComponent->bConstrainToPlane = true;
	MovementComponent->SetPlaneConstraintNormal(FVector::UpVector);
}

void ACppCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!FMath::IsNearlyEqual(SpringArmComponent->TargetArmLength, DesiredZoom, 0.5f))
	{
		SpringArmComponent->TargetArmLength = FMath::FInterpTo(
			SpringArmComponent->TargetArmLength,
			DesiredZoom,
			DeltaTime,
			ZoomInterpSpeed);
	}

	SmoothedPanInput.X = FMath::FInterpTo(SmoothedPanInput.X, RawPanInput.X, DeltaTime, PanInterpSpeed);
	SmoothedPanInput.Y = FMath::FInterpTo(SmoothedPanInput.Y, RawPanInput.Y, DeltaTime, PanInterpSpeed);
	RawPanInput = FVector2D::ZeroVector;

	if (!SmoothedPanInput.IsNearlyZero(0.001f))
	{
		const FVector2D ClampedInput = SmoothedPanInput.GetClampedToMaxSize(1.4142f);
		const FRotator YawOnly(0.f, CameraComponent ? CameraComponent->GetComponentRotation().Yaw : 0.f, 0.f);
		const FVector Forward = FRotationMatrix(YawOnly).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawOnly).GetUnitAxis(EAxis::Y);
		const FVector Direction = Forward * ClampedInput.Y + Right * ClampedInput.X;
		const float ZoomScale = DesiredZoom / 2000.f;
		AddMovementInput(Direction.GetSafeNormal(), Direction.Size() * PanSpeed * ZoomScale);
	}
}

float ACppCameraPawn::GetZoom() const
{
	return SpringArmComponent->TargetArmLength;
}

float ACppCameraPawn::GetDesiredZoom() const
{
	return DesiredZoom;
}

void ACppCameraPawn::AddPanInput(const FVector2D& InInput)
{
	RawPanInput += InInput;
}

void ACppCameraPawn::AddZoom(float InDelta)
{
	DesiredZoom = FMath::Clamp(DesiredZoom - InDelta * ZoomStep, MinZoom, MaxZoom);
}
