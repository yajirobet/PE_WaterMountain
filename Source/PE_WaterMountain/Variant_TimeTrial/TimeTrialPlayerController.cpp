// Copyright Epic Games, Inc. All Rights Reserved.


#include "TimeTrialPlayerController.h"
#include "TimeTrialUI.h"
#include "Engine/World.h"
#include "TimeTrialGameMode.h"
#include "TimeTrialTrackGate.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "PE_WaterMountainUI.h"
#include "PE_WaterMountainPawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "PE_WaterMountain.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Widgets/Input/SVirtualJoystick.h"

void ATimeTrialPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only spawn UI on local player controllers
	if (IsLocalPlayerController())
	{
		if (SVirtualJoystick::ShouldDisplayTouchInterface())
		{
			// spawn the mobile controls widget
			MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

			if (MobileControlsWidget)
			{
				// add the controls to the player screen
				MobileControlsWidget->AddToPlayerScreen(0);

			} else {

				UE_LOG(LogPE_WaterMountain, Error, TEXT("Could not spawn mobile controls widget."));

			}
		}

		// create the UI widget
		UIWidget = CreateWidget<UTimeTrialUI>(this, UIWidgetClass);

		if (UIWidget)
		{
			UIWidget->AddToViewport(0);

			// subscribe to the race start delegate
			UIWidget->OnRaceStart.AddDynamic(this, &ATimeTrialPlayerController::StartRace);

		} else {

			UE_LOG(LogPE_WaterMountain, Error, TEXT("Could not spawn Time Trial UI widget."));

		}
		

		// spawn the UI widget and add it to the viewport
		VehicleUI = CreateWidget<UPE_WaterMountainUI>(this, VehicleUIClass);

		if (VehicleUI)
		{
			VehicleUI->AddToViewport(0);

		} else {

			UE_LOG(LogPE_WaterMountain, Error, TEXT("Could not spawn vehicle UI widget."));

		}
	}

}

void ATimeTrialPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}

void ATimeTrialPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// get a pointer to the controlled pawn
	VehiclePawn = CastChecked<APE_WaterMountainPawn>(InPawn);

	// subscribe to the pawn's OnDestroyed delegate
	VehiclePawn->OnDestroyed.AddDynamic(this, &ATimeTrialPlayerController::OnPawnDestroyed);

	// disable input on the pawn if the race hasn't started yet
	if (!bRaceStarted)
	{
		VehiclePawn->DisableInput(this);
	}	
}

void ATimeTrialPlayerController::Tick(float Delta)
{
	Super::Tick(Delta);

	if (IsValid(VehiclePawn) && IsValid(VehicleUI))
	{
		VehicleUI->UpdateSpeed(VehiclePawn->GetChaosVehicleMovement()->GetForwardSpeed());
		VehicleUI->UpdateGear(VehiclePawn->GetChaosVehicleMovement()->GetCurrentGear());
	}
}

void ATimeTrialPlayerController::StartRace()
{
	// get the finish line from the game mode
	if (ATimeTrialGameMode* GM = Cast<ATimeTrialGameMode>(GetWorld()->GetAuthGameMode()))
	{
		SetTargetGate(GM->GetFinishLine()->GetNextMarker());
	}

	// raise the race started flag so any respawned vehicles start with controls unlocked 
	bRaceStarted = true;

	// start the first lap
	CurrentLap = 0;
	IncrementLapCount();

	// enable input on the pawn
	GetPawn()->EnableInput(this);
}

void ATimeTrialPlayerController::IncrementLapCount()
{
	// increment the lap counter
	++CurrentLap;

	// update the UI
	UIWidget->UpdateLapCount(CurrentLap, GetWorld()->GetTimeSeconds());
}

ATimeTrialTrackGate* ATimeTrialPlayerController::GetTargetGate()
{
	return TargetGate.Get();
}

void ATimeTrialPlayerController::SetTargetGate(ATimeTrialTrackGate* Gate)
{
	TargetGate = Gate;
}

void ATimeTrialPlayerController::OnPawnDestroyed(AActor* DestroyedPawn)
{
	// find the player start
	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), ActorList);

	if (ActorList.Num() > 0)
	{
		// spawn a vehicle at the player start
		const FTransform SpawnTransform = ActorList[0]->GetActorTransform();

		if (APE_WaterMountainPawn* RespawnedVehicle = GetWorld()->SpawnActor<APE_WaterMountainPawn>(VehiclePawnClass, SpawnTransform))
		{
			// possess the vehicle
			Possess(RespawnedVehicle);
		}
	}
}
