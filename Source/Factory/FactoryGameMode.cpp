// Copyright Epic Games, Inc. All Rights Reserved.

#include "FactoryGameMode.h"
#include "FactoryPlayerController.h"
#include "PlayerCharacter.h"

AFactoryGameMode::AFactoryGameMode()
{
	// no pawn by default
	DefaultPawnClass = APlayerCharacter::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AFactoryPlayerController::StaticClass();
}
