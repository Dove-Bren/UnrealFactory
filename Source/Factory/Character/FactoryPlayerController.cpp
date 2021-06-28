// Copyright Epic Games, Inc. All Rights Reserved.

#include "FactoryPlayerController.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Framework/Application/SlateApplication.h"
//#include "SlateApplication.h"

#include "Factory/Character/PlayerCharacter.h"
#include "Factory/Character/PlacingActor.h"
#include "Factory/Logical/Item.h"

AFactoryPlayerController::AFactoryPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	bEnableTouchEvents = true;
	DefaultClickTraceChannel = ECC_Pawn;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	HUDManager = CreateDefaultSubobject<UHUDManager>(TEXT("HUDManager"));
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

	if (ActiveMouseItemActor)
	{
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
		bool bHit = false;
		if (LocalPlayer && LocalPlayer->ViewportClient)
		{
			FVector2D MousePosition;
			if (LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
			{
				FHitResult HitResult;
				FVector Origin;
				FVector BoxExtent;
				FCollisionQueryParams Params;

				Params.bTraceComplex = true;
				Params.AddIgnoredActor(ActiveMouseItemActor);

				if (GetHitResultAtScreenPosition(MousePosition, ECollisionChannel::ECC_Visibility, Params, HitResult))
				{
					// Center actor on cursor, which means getting bounds
					ActiveMouseItemActor->GetActorBounds(false, Origin, BoxExtent, true);
					BoxExtent.Z = 0; // Center horizontally but leave bottom vertical where it's at

					ActiveMouseItemActor->SetActorLocation(HitResult.Location - BoxExtent);
				}
			}
		}
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

w	this->ActiveItemClicked();
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
			if (!ControlledCharacter->CanSprint() || !IsSprinting())
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
			if (!ControlledCharacter->CanSprint() || !IsSprinting())
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
	if (ControlledCharacter && ControlledCharacter->CanJump())
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

void AFactoryPlayerController::GoToShop(AShop *Shop, EGamePlatform Platform, bool bTeleport)
{
	APlayerCharacter *ControlledCharacter = dynamic_cast<APlayerCharacter *>(this->GetCharacter());
	if (ControlledCharacter)
	{
		ControlledCharacter->SetShop(Shop, Platform, bTeleport);
	}

	HUDManager->SetGamePlatform(Platform);
}

void AFactoryPlayerController::BeginPlay(void)
{
	Super::BeginPlay();
}

void AFactoryPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	APlayerCharacter *ControlledCharacter = dynamic_cast<APlayerCharacter *>(this->GetCharacter());
	if (ControlledCharacter)
	{
		HUDManager->SetCharacter(ControlledCharacter);
	}
}

void AFactoryPlayerController::SetActiveMouseItem(UInventorySlotRef *ItemSlotRef)
{
	FActorSpawnParameters Parameters{};

	ClearActiveItem();

	if (ItemSlotRef && ItemSlotRef->GetItem())
	{
		UItem *Item = ItemSlotRef->GetItem();

		if (Item && Item->GetType()->GetMesh())
		{
			ActiveMouseItem = ItemSlotRef;
			Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ActiveMouseItemActor = GetWorld()->SpawnActor<APlacingActor>(Parameters);
			ActiveMouseItemActor->Setup(this, Item->GetType()->GetMesh());
		}
	}
}

void AFactoryPlayerController::ClearActiveItem()
{
	if (ActiveMouseItemActor && ActiveMouseItemActor->IsValidLowLevel())
	{
		ActiveMouseItemActor->GetWorld()->DestroyActor(ActiveMouseItemActor);// ... I guess not since the controller won't die until the very end?
	}
	ActiveMouseItemActor = nullptr;
	ActiveMouseItem = nullptr;
}

AFactoryPlayerController::~AFactoryPlayerController()
{
	ClearActiveItem();
}

void AFactoryPlayerController::ActiveItemClicked()
{
	if (ActiveMouseItem && ActiveMouseItemActor)
	{
		if (this->ActiveMouseItem->IsValid())
		{
			ActiveMouseItem->RemoveItems(1);
		}

		if (!this->ActiveMouseItem->IsValid())
		{
			ClearActiveItem();
		}
	}
}