// Copyright Epic Games, Inc. All Rights Reserved.

#include "FactoryPlayerController.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Framework/Application/SlateApplication.h"
//#include "SlateApplication.h"

#include "Factory/GameEnums.h"
#include "Factory/Character/PlayerCharacter.h"
#include "Factory/Character/PlacingActor.h"
#include "Factory/Logical/Item.h"
#include "Factory/Logical/LogicalShop.h"
#include "Factory/Logical/LogicalPlatform.h"

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
	InputComponent->BindAxis("RotatePlacement", this, &AFactoryPlayerController::RotatePlacement);
	
}

void AFactoryPlayerController::PlayerTick(float DeltaSeconds)
{
	Super::PlayerTick(DeltaSeconds);

	APlayerCharacter *PlayerCharacter = Cast<APlayerCharacter>(GetCharacter());

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

	bActiveMouseItemValid = false;
	if (ActiveMouseItemActor)
	{
		if (PlayerCharacter && PlayerCharacter->GetShop())
		{
			AShop *ShopBuilding = PlayerCharacter->GetShop();
			UPlatform *PlatformBuilding = PlayerCharacter->GetPlatform();
			ULogicalShop *Shop = ShopBuilding->GetLogicalShop();
			ULogicalPlatform *Platform = PlatformBuilding->GetLogicalPlatform();

			if (Shop && Platform)
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

						if (GetHitResultAtScreenPosition(MousePosition, ECollisionChannel::ECC_Visibility, Params, HitResult)
							&& FMath::Abs(HitResult.Location.Z - PlatformBuilding->GetComponentLocation().Z) < 10)
						{
							// Query platform for grid position and what may already be there
							FGridPosition GridPos = Platform->GetGridPosFromWorld(HitResult.Location.X, HitResult.Location.Y);
							FLocalLayout Layout = Platform->GetComponent(GridPos);
							FVector WorldCenterPos = Platform->GetWorldPosFromGrid(GridPos, true);

							// Adjust hitpos to center of cell
							HitResult.Location.X = WorldCenterPos.X;
							HitResult.Location.Y = WorldCenterPos.Y;
							HitResult.Location.Z = PlatformBuilding->GetComponentLocation().Z;

							ActiveMouseItemActor->SetActorLocation(HitResult.Location);

							// Rotate according to rotation
							FRotator Rotation = GetRotationFromDirection(this->ActiveMouseItemDirection);
							ActiveMouseItemActor->SetActorRotation(Rotation);

							// For now, treat as valid if there's nothing there already
							bActiveMouseItemValid = !Layout.Center;
						}
					}
				}
			}
		}

		const bool bValid = bActiveMouseItemValid;
		ActiveMouseItemActor->ForEachComponent<UPrimitiveComponent>(true, [bValid](UPrimitiveComponent *Component) {
			Component->SetRenderCustomDepth(true);
			Component->SetCustomDepthStencilValue(GetColorIndex(bValid ? EStandardColors::GREEN : EStandardColors::RED));
		});
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

	this->ActiveItemClicked();
}

void AFactoryPlayerController::SecondaryClick()
{
	this->ClearActiveItem();
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

void AFactoryPlayerController::RotatePlacement(float Value)
{
	APlayerCharacter *ControlledCharacter = Cast<APlayerCharacter>(this->GetCharacter());
	if (ControlledCharacter && Value != 0 && ActiveMouseItem)
	{
		bool bForward = Value > 0;
		Value = FMath::Abs(Value);
		while (Value-- > 0)
		{
			ActiveMouseItemDirection = RotateDirection(ActiveMouseItemDirection, !bForward);
		}
	}
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

			// Set initial direction based on camera rotation
			const FRotator Rotation = GetControlRotation();
			ActiveMouseItemDirection = GetDirectionFromYaw(Rotation.Yaw);
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
	ActiveMouseItemDirection = EDirection::NORTH;
}

AFactoryPlayerController::~AFactoryPlayerController()
{
	ClearActiveItem();
}

void AFactoryPlayerController::ActiveItemClicked()
{
	APlayerCharacter *PlayerCharacter = dynamic_cast<APlayerCharacter *>(this->GetCharacter());
	if (ActiveMouseItem && ActiveMouseItemActor && bActiveMouseItemValid && PlayerCharacter && PlayerCharacter->GetShop())
	{
		if (this->ActiveMouseItem->IsValid())
		{
			UPlatform *PlatformBuilding = PlayerCharacter->GetPlatform();
			ULogicalPlatform *Platform = PlatformBuilding->GetLogicalPlatform();
			FVector PlacePos = ActiveMouseItemActor->GetActorLocation();

			FGridPosition GridPos = Platform->GetGridPosFromWorld(PlacePos.X, PlacePos.Y);
			FLocalLayout Layout = Platform->GetComponent(GridPos);
			
			// Last check
			if (!Layout.Center)
			{
				UItem *UsedItem = ActiveMouseItem->GetItem();
				ULogicalPlatformComponent *Component = UsedItem->GetType()->SpawnPlatformComponent(
					PlayerCharacter->GetShopPlatformType(),
					Platform,
					PlayerCharacter,
					UsedItem
				);

				if (Component)
				{
					UItem *ConsumedItem = ActiveMouseItem->RemoveItems(1); //APlayerCharacter *PlacingCharacter, UItem *Item
					Component->OnInitialPlacement(PlayerCharacter, ConsumedItem, ActiveMouseItemDirection);
					Platform->AddComponent(GridPos, Component);

					// Spawn in world, too
					Component->SpawnWorldComponent(PlatformBuilding);
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Tried to place component but at the last minute we found one was there!"));
			}

			
		}

		if (!this->ActiveMouseItem->IsValid())
		{
			ClearActiveItem();
		}
	}
}