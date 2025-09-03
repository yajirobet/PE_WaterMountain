// Copyright Epic Games, Inc. All Rights Reserved.

#include "PE_WaterMountainWheelFront.h"
#include "UObject/ConstructorHelpers.h"

UPE_WaterMountainWheelFront::UPE_WaterMountainWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}