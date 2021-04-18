// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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

private:
	bool bSprintLock;
	bool bSprintMod;

};


