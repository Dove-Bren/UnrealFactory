#pragma once

// Base class for the full screen HUDs for each platform.

#include "CoreMinimal.h"

#include "Factory/UI/FactoryHUDWidget.h"

#include "Factory/Character/PlayerCharacter.h"
#include "Factory/Building/Shop.h"

#include "FactoryHUDBase.generated.h"

UCLASS()
class UFactoryHUDBase : public UFactoryHUDWidget
{
	GENERATED_BODY()

public:

	// Create and return the appropriate inventory overlay for this HUD element.
	// The returned element will be displayed as a screen.
	// This func is only called when appropriate and we need a new screen -- not when one's simply being closed.
	// Returning nothing (or not implementing this function) makes no inventory open.
	UFUNCTION(BlueprintImplementableEvent)
	UFactoryHUDWidget *GetInventoryOverlay();
};