// Copyright Epic Games, Inc. All Rights Reserved.

#include "PE_WaterMountainGameMode.h"
#include "PE_WaterMountainPlayerController.h"

APE_WaterMountainGameMode::APE_WaterMountainGameMode()
{
	PlayerControllerClass = APE_WaterMountainPlayerController::StaticClass();
}
