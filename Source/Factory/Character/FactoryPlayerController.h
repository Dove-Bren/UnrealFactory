// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factory/GameEnums.h"
#include "Factory/Building/Shop.h"
#include "Factory/UI/HudManager.h"
#include "Factory/Logical/Inventory.h"
#include "GameFramework/PlayerController.h"
#include "FactoryPlayerController.generated.h"

typedef class APlacingActor;

/** PlayerController class used to enable cursor */
UCLASS()
class AFactoryPlayerController : public APlayerController
{
	GENERATED_BODY()

	friend class APlacingActor;

public:
	AFactoryPlayerController();
	virtual ~AFactoryPlayerController();

	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	// Move to a shop platform
	void GoToShop(AShop *Shop, EGamePlatform Platform = EGamePlatform::STORE, bool bTeleport = true);

	// Get the Hud Manager
	UFUNCTION(BlueprintCallable)
	UHUDManager *GetHudManager() const { return HUDManager; }

	// Get the current item (ref) the player has selected, if any
	UFUNCTION(BlueprintCallable)
	UInventorySlotRef *GetActiveMouseItem() const { return ActiveMouseItem; }

	// Set the currently selected item
	UFUNCTION(BlueprintCallable)
	void SetActiveMouseItem(UInventorySlotRef *ItemSlotRef);

protected:

	void OnResetVR();
	void PrimaryClick();
	void SecondaryClick();
	void MouseYaw(float Value);
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Jump();
	void StopJumping();
	void StartSprinting();
	void StopSprinting();
	void ToggleAlwaysSprint();
	void Trace(const FVector& Start, const FVector& End, bool bDrawDebugHelpers);

	void SetSprintLock(bool bOn);
	bool IsSprinting();

	virtual void OnPossess(APawn* aPawn) override;

private:
	bool bSprintLock;
	bool bSprintMod;

	// The HUD Manager, which manages which HUD is up and handles changing them out
	UPROPERTY(VisibleAnywhere)
	UHUDManager *HUDManager;

	// What item (if any; can be nullptr) the character has selected and is using
	// the mouse to position/select/etc.
	UPROPERTY(VisibleAnywhere)
	UInventorySlotRef *ActiveMouseItem;

	// Actor in world to match ActiveMouseItem
	APlacingActor *ActiveMouseItemActor;

	// Internal notification from the placing actor that it has been clicked
	virtual void ActiveItemClicked();

	void ClearActiveItem();

};


