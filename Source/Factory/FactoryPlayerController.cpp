// Copyright Epic Games, Inc. All Rights Reserved.

#include "FactoryPlayerController.h"

AFactoryPlayerController::AFactoryPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}
