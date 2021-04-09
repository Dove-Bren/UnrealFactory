// Copyright Epic Games, Inc. All Rights Reserved.

#include "FactoryGameMode.h"
#include "FactoryPlayerController.h"
#include "FactoryPawn.h"

AFactoryGameMode::AFactoryGameMode()
{
	// no pawn by default
	DefaultPawnClass = AFactoryPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AFactoryPlayerController::StaticClass();
}
