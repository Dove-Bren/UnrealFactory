#pragma once

// Convenience for handling which HUD menu is displayed and the transition between them

#pragma once

#include "CoreMinimal.h"

#include "Factory/GameEnums.h"
#include "Factory/UI/FactoryHUDWidget.h"
#include "Factory/UI/FactoryHUDBase.h"

#include "HudManager.generated.h"

typedef class APlayerCharacter APlayerCharacter;

UCLASS()
class UHUDManager : public UObject
{
	GENERATED_BODY()

public:
	UHUDManager();
	~UHUDManager();

	// Update the HUD displayed to match the provided platform
	UFUNCTION(BlueprintCallable)
	void SetGamePlatform(EGamePlatform Platform); // TODO some cool transition?

	// Check whether the HUD manager has the game paused
	UFUNCTION(BlueprintCallable)
	bool IsGamePaused();

	// Pause the game and show the pause screen
	UFUNCTION(BlueprintCallable)
	void PauseGame();

	// Handle a request to toggle showing the inventory (or close current screen)
	UFUNCTION(BlueprintCallable)
	void ToggleInventory();

	// Update the character the huds represent
	UFUNCTION(BlueprintCallable)
	void SetCharacter(APlayerCharacter *Character);

	// Update what popup (if any -- can be null) to display.
	UFUNCTION(BlueprintCallable)
	void SetScreen(UFactoryHUDWidget *Screen);

	// Check whether any screen is currently up
	UFUNCTION(BlueprintCallable)
	bool HasScreen() { return !!CurrentScreen; }

	UFUNCTION(BlueprintCallable)
	bool IsScreenShown(UFactoryHUDWidget *Screen) { return Screen == this->CurrentScreen; }

protected:

	APlayerCharacter *CurrentCharacter;
	UFactoryHUDBase *CurrentHud;

	UFactoryHUDWidget *CurrentScreen;

private:

	TSubclassOf<class UFactoryHUDBase> MineHUD_Class;
	TSubclassOf<class UFactoryHUDBase> FactoryHUD_Class;
	TSubclassOf<class UFactoryHUDBase> StoreHUD_Class;

};