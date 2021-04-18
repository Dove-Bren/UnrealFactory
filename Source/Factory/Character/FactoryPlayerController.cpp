// Copyright Epic Games, Inc. All Rights Reserved.

#include "FactoryPlayerController.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Framework/Application/SlateApplication.h"
//#include "SlateApplication.h"

#include "PlayerCharacter.h"

AFactoryPlayerController::AFactoryPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AFactoryPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("ResetVR", EInputEvent::IE_Pressed, this, &AFactoryPlayerController::OnResetVR);
	InputComponent->BindAction("PrimaryClick", EInputEvent::IE_Pressed, this, &AFactoryPlayerController::PrimaryClick);
	InputComponent->BindAction("SecondaryClick", EInputEvent::IE_Pressed, this, &AFactoryPlayerController::SecondaryClick);
	InputComponent->BindAxis("MouseYaw", this, &AFactoryPlayerController::MouseYaw);
	InputComponent->BindAxis("MoveForward", this, &AFactoryPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFactoryPlayerController::MoveRight);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AFactoryPlayerController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &AFactoryPlayerController::StopJumping);
	InputComponent->BindAction("Sprint", IE_Pressed, this, &AFactoryPlayerController::StartSprinting);
	InputComponent->BindAction("Sprint", IE_Released, this, &AFactoryPlayerController::StopSprinting);
	InputComponent->BindAction("AlwaysSprint", IE_Pressed, this, &AFactoryPlayerController::StopSprinting);
}

void AFactoryPlayerController::PlayerTick(float DeltaSeconds)
{
	Super::PlayerTick(DeltaSeconds);

	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (UCameraComponent* OurCamera = GetViewTarget()->FindComponentByClass<UCameraComponent>())
		{
			FVector Start = OurCamera->GetComponentLocation();
			FVector End = Start + (OurCamera->GetComponentRotation().Vector() * 8000.0f);
			Trace(Start, End, true);
		}
	}
	else
	{
		FVector Start, Dir, End;
		DeprojectMousePositionToWorld(Start, Dir);
		End = Start + (Dir * 8000.0f);
		Trace(Start, End, false);
	}
}

void AFactoryPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AFactoryPlayerController::PrimaryClick()
{
	/*if (CurrentBlockFocus)
	{
		CurrentBlockFocus->HandleClicked();
	}*/
}

void AFactoryPlayerController::SecondaryClick()
{
	;
}

void AFactoryPlayerController::MouseYaw(float Value)
{
	this->AddYawInput(Value);
}

bool AFactoryPlayerController::IsSprinting()
{
	// Caps lock/sprint lock is base state. Shift flips it.
	bool bRunning = this->bSprintLock;
	if (this->bSprintMod)
	{
		bRunning = !bRunning;
	}

	return bRunning;
}

void AFactoryPlayerController::MoveForward(float Value)
{
	APlayerCharacter *ControlledCharacter = dynamic_cast<APlayerCharacter *>(this->GetCharacter());
	if (ControlledCharacter /*&& ControlledCharacter->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling*/)
	{
		if (Value != 0.0f)
		{
			// Modify value based on sprint
			if (!IsSprinting())
			{
				Value *= .5f;
			}

			// find out which way is forward
			const FRotator Rotation = GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			ControlledCharacter->AddMovementInput(Direction, Value);
		}
	}
}

void AFactoryPlayerController::MoveRight(float Value)
{
	APlayerCharacter *ControlledCharacter = dynamic_cast<APlayerCharacter *>(this->GetCharacter());
	if (ControlledCharacter /* && ControlledCharacter->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling*/)
	{
		if (Value != 0.0f)
		{
			// Modify value based on sprint
			if (!IsSprinting())
			{
				Value *= .5f;
			}

			// find out which way is right
			const FRotator Rotation = GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			ControlledCharacter->AddMovementInput(Direction, Value);
		}
	}
}

void AFactoryPlayerController::Jump()
{
	APlayerCharacter *ControlledCharacter = dynamic_cast<APlayerCharacter *>(this->GetCharacter());
	if (ControlledCharacter)
	{
		ControlledCharacter->Jump();
	}
}

void AFactoryPlayerController::StopJumping()
{
	APlayerCharacter *ControlledCharacter = dynamic_cast<APlayerCharacter *>(this->GetCharacter());
	if (ControlledCharacter)
	{
		ControlledCharacter->StopJumping();
	}
}

void AFactoryPlayerController::ToggleAlwaysSprint()
{
	// Actually just detect capslock status and then set our state based on that
	SetSprintLock(FSlateApplication::Get().GetModifierKeys().AreCapsLocked());
}

void AFactoryPlayerController::StartSprinting()
{
	this->bSprintMod = true;
}

void AFactoryPlayerController::StopSprinting()
{
	this->bSprintMod = false;
}

void AFactoryPlayerController::SetSprintLock(bool bOn)
{
	this->bSprintLock = bOn;
}

void AFactoryPlayerController::Trace(const FVector& Start, const FVector& End, bool bDrawDebugHelpers)
{
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	if (bDrawDebugHelpers)
	{
		DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red);
		DrawDebugSolidBox(GetWorld(), HitResult.Location, FVector(20.0f), FColor::Red);
	}

	/*bool bHit = false;
	if (HitResult.Actor.IsValid())
	{
		AFactoryBlock* HitBlock = Cast<AFactoryBlock>(HitResult.Actor.Get());
		if (CurrentBlockFocus != HitBlock)
		{
			if (CurrentBlockFocus)
			{
				CurrentBlockFocus->Highlight(false);
			}
			if (HitBlock)
			{
				HitBlock->Highlight(true);
				bHit = true;
			}
			CurrentBlockFocus = HitBlock;
		}
	}
	else if (CurrentBlockFocus)
	{
		CurrentBlockFocus->Highlight(false);
		CurrentBlockFocus = nullptr;
	}


	if (bHit)
	{
		FTransform transform(FVector(0, 0, 5));
		this->RootComponent->AddWorldTransform(transform);

		AFactoryBlockGrid *g_Grid = AFactoryBlockGrid::Get();
		if (g_Grid)
		{
			g_Grid->Score += 2;
		}
	}*/
}