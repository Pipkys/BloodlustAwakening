#include "BloodlustAwakening/Player/CppPlayerController.h"

#include "BloodlustAwakening/Core/Physics/CppCollisionChannels.h"
#include "BloodlustAwakening/GameMode/CppGameplayStatics.h"
#include "BloodlustAwakening/Interface/CppAttacker.h"
#include "BloodlustAwakening/Interface/CppEntity.h"
#include "BloodlustAwakening/Level/Grid/CppCellGrid.h"
#include "BloodlustAwakening/Player/CppCameraPawn.h"
#include "BloodlustAwakening/Subsystems/CppBuildSubsystem.h"

#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Kismet/KismetSystemLibrary.h"

ACppPlayerController::ACppPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
}

void ACppPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, MappingPriority);
			}
		}
	}

	static bool bViewTargetLogged = false;
	APawn* ControlledPawn = GetPawn();
	AActor* CurrentViewTarget = GetViewTarget();
	if (CurrentViewTarget != ControlledPawn)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("ACppPlayerController::PlayerTick: view target '%s' (%s) != possessed pawn '%s', forcing pawn view target"),
			*GetNameSafe(CurrentViewTarget),
			*GetNameSafe(CurrentViewTarget->GetClass()),
			*GetNameSafe(ControlledPawn));

		SetViewTargetWithBlend(ControlledPawn, 0.f);
		bViewTargetLogged = true;
	}
	else if (!bViewTargetLogged)
	{
		UE_LOG(LogTemp, Log, TEXT("ACppPlayerController::PlayerTick: view target ok = '%s'"), *GetNameSafe(ControlledPawn));
		bViewTargetLogged = true;
	}
}

void ACppPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComponent)
	{
		return;
	}

	if (IA_PrimaryPressed)
	{
		EnhancedInputComponent->BindAction(IA_PrimaryPressed, ETriggerEvent::Triggered, this, &ACppPlayerController::OnPrimaryPressed);
	}
	if (IA_Pan)
	{
		EnhancedInputComponent->BindAction(IA_Pan, ETriggerEvent::Triggered, this, &ACppPlayerController::OnPan);
	}
	if (IA_Zoom)
	{
		EnhancedInputComponent->BindAction(IA_Zoom, ETriggerEvent::Triggered, this, &ACppPlayerController::OnZoom);
	}
}

void ACppPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	MarkCellsForBuildingBlueprint();
}

void ACppPlayerController::OnPrimaryPressed(const FInputActionValue& Value)
{
	BuildSelectedBuilding();
}

void ACppPlayerController::OnPan(const FInputActionValue& Value)
{
	if (ACppCameraPawn* CameraPawn = Cast<ACppCameraPawn>(GetPawn()))
	{
		const FVector2D Axis = Value.Get<FVector2D>();
		CameraPawn->AddPanInput(Axis);
	}
}

void ACppPlayerController::OnZoom(const FInputActionValue& Value)
{
	if (ACppCameraPawn* CameraPawn = Cast<ACppCameraPawn>(GetPawn()))
	{
		CameraPawn->AddZoom(Value.Get<float>());
	}
}

void ACppPlayerController::SetBuildingBlueprint(TSubclassOf<ACppBuilding> InBuildingBlueprint)
{
	CurrentBuildingBlueprint = InBuildingBlueprint;
}

bool ACppPlayerController::IsBuildingBlueprintSelected() const
{
	return UKismetSystemLibrary::IsValidClass(CurrentBuildingBlueprint);
}

void ACppPlayerController::MarkCellsForBuildingBlueprint() const
{
	if (!IsBuildingBlueprintSelected())
	{
		return;
	}

	FHitResult Hit;
	bool bHit = GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	if (!bHit)
	{
		for (const TObjectPtr<ACppCell>& MarkedCell : MarkedCells)
		{
			MarkedCell->UnmarkCell();
		}
		MarkedCells.Empty();
		return;
	}

	AActor* HitActor = Hit.GetActor();
	ACppCell* HitCell = Cast<ACppCell>(HitActor);
	for (const TObjectPtr<ACppCell>& MarkedCell : MarkedCells)
	{
		MarkedCell->UnmarkCell();
	}
	MarkedCells.Empty();
	if (!HitCell)
	{
		return;
	}

	ACppCellGrid* Grid = HitCell->GetGrid();
	FVector2f CellCoords = HitCell->Coords;
	float BuildingSizeInCells = (float)((ACppBuilding*)CurrentBuildingBlueprint->GetDefaultObject())->GetSizeInCells();
	float BuildingSizeOffsetInCells = (BuildingSizeInCells - 1.f) / 2.f;
	for (int32 XIndex = FMath::CeilToInt32(CellCoords.X - BuildingSizeOffsetInCells);
		XIndex <= FMath::CeilToInt32(CellCoords.X + BuildingSizeOffsetInCells);
		++XIndex)
	{
		for (int32 YIndex = FMath::CeilToInt32(CellCoords.Y - BuildingSizeOffsetInCells);
			YIndex <= FMath::CeilToInt32(CellCoords.Y + BuildingSizeOffsetInCells);
			++YIndex)
		{
			FIntPoint XYPoint{ XIndex, YIndex };
			if (TObjectPtr<ACppCell>* NearCell = Grid->Cells.Find(XYPoint))
			{
				(*NearCell)->MarkCell();
				MarkedCells.Add(*NearCell);
			}
		}
	}
}

void ACppPlayerController::BuildSelectedBuilding() const
{
	if (!IsBuildingBlueprintSelected())
	{
		return;
	}

	int32 BuildingSizeInCells = ((ACppBuilding*)CurrentBuildingBlueprint->GetDefaultObject())->GetSizeInCells();
	if (MarkedCells.Num() != BuildingSizeInCells * BuildingSizeInCells)
	{
		return;
	}

	for (TObjectPtr<ACppCell> MarkedCell : MarkedCells)
	{
		if (MarkedCell->bIsOccupied)
		{
			return;
		}
	}

	UCppBuildSubsystem::BuildTower(GetOwner(),
		MarkedCells,
		(TSubclassOf<ACppTower>)CurrentBuildingBlueprint,
		{},
		"BloodRace");
}
