// Copyright Epic Games, Inc. All Rights Reserved.

#include "PE_WaterMountainWheelRear.h"
#include "UObject/ConstructorHelpers.h"

UPE_WaterMountainWheelRear::UPE_WaterMountainWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}