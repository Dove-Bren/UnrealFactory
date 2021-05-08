// Copyright Epic Games, Inc. All Rights Reserved.

#include "FactoryGameMode.h"
#include "Factory/Character/FactoryPlayerController.h"
#include "Factory/Character/PlayerCharacter.h"

AFactoryGameMode::AFactoryGameMode()
{
	// no pawn by default
	DefaultPawnClass = APlayerCharacter::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AFactoryPlayerController::StaticClass();
}
