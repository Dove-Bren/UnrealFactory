// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factory/GameEnums.h"
#include "Factory/Building/Shop.h"
#include "Factory/UI/HudManager.h"
#include "GameFramework/PlayerController.h"
#include "FactoryPlayerController.generated.h"

/** PlayerController class used to enable cursor */
UCLASS()
class AFactoryPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFactoryPlayerController();

	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	// Move to a shop platform
	void GoToShop(AShop *Shop, EGamePlatform Platform = EGamePlatform::STORE, bool bTeleport = true);

	// Get the Hud Manager
	UFUNCTION(BlueprintCallable)
	UHUDManager *GetHudManager() { return HUDManager; }

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

	UPROPERTY(VisibleAnywhere)
	UHUDManager *HUDManager;

};


