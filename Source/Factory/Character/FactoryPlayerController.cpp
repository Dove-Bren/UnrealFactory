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
#include "Factory/Logical/Inventory/Item.h"
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
	InputComponent->BindAction("ToggleInventory", IE_Pressed, this->HUDManager, &UHUDManager::ToggleInventory);
	
}

FVector2D AFactoryPlayerController::GetMousePositon()
{
	FVector2D MousePosition(0, 0);
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	if (LocalPlayer && LocalPlayer->ViewportClient)
	{
		LocalPlayer->ViewportClient->GetMousePosition(MousePosition);
	}
	return MousePosition;
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

	// If screen open and tied to a position, close if we've moved too far away
	if (bAutoCloseScreenUsesLocation)
	{
		if (!GetHudManager()->IsScreenShown(AutoCloseScreen))
		{
			AutoCloseScreen = nullptr;
			bAutoCloseScreenUsesLocation = false;
		}
		else if (FVector::Dist(PlayerCharacter->GetActorLocation(), AutoCloseScreenSourceLocation) > PlayerCharacter->GetMaxReach())
		{
			GetHudManager()->SetScreen(nullptr);
			bAutoCloseScreenUsesLocation = false;

			// Could do a better job of checking if screen was related to selected item.
			// If I ever do a hotbar or something this will have to change.
			// Since right now you can only select if you have an inventory screen open,
			// assume it's okay to clear out the active item
			this->ClearActiveItem();

			AutoCloseScreen = nullptr;
		}
	}

	/*
UFactoryHUDWidget *AutoCloseScreen;
	FVector AutoCloseScreenSourceLocation;
	bool bAutoCloseScreenUsesLocation;
*/

	// Update placing logic if we're in placement mode
	if (ActiveMouseItemActor && PlayerCharacter && PlayerCharacter->GetShop())
	{
		AShop *ShopBuilding = PlayerCharacter->GetShop();
		UPlatform *PlatformBuilding = PlayerCharacter->GetPlatform();
		ULogicalShop *Shop = ShopBuilding->GetLogicalShop();
		ULogicalPlatform *Platform = PlatformBuilding->GetLogicalPlatform();

		check(Shop && Platform);

		// First, try to figure out where cursor is at
		bool bValidLocation = false;
		FVector WorldPos;
		FGridPosition GridPos;

		{
			FVector2D MousePosition = GetMousePositon();
			FHitResult HitResult;
			FVector Origin;
			FVector BoxExtent;
			FCollisionQueryParams Params;

			Params.bTraceComplex = true;
			Params.AddIgnoredActor(ActiveMouseItemActor);

			if (GetHitResultAtScreenPosition(MousePosition, ECollisionChannel::ECC_Visibility, Params, HitResult)
				&& HitResult.bBlockingHit
				&& FMath::Abs(HitResult.Location.Z - PlatformBuilding->GetComponentLocation().Z) < 100)
			{
				// Raytrace hit something!
				GridPos = Platform->GetGridPosFromWorld(HitResult.Location.X, HitResult.Location.Y);
				WorldPos = Platform->GetWorldPosFromGrid(GridPos, true);
				WorldPos.Z = PlatformBuilding->GetComponentLocation().Z;

				// Make sure this is in the limits of the platform but otherwise call it good
				bValidLocation = Platform->IsGridPosValid(GridPos);
			}
		}

		if (bValidLocation)
		{
			// Get logical info around cell
			FLocalLayout Layout = Platform->GetComponent(GridPos);

			// Update actor location
			ActiveMouseItemActor->SetActorLocation(WorldPos);

			// Update logical info about cell if it's not our cached cell/direction
			if (bActiveMouseItemCacheDirty
				|| (GridPos != ActiveMouseItemCachePos || ActiveMouseItemDirection != ActiveMouseItemCacheDirection))
			{
				ActiveMouseItemCachePos = GridPos;
				ActiveMouseItemCacheDirection = ActiveMouseItemDirection;
				bActiveMouseItemCacheDirty = false;

				// Build map of nearby connection opportunities and update actor
				FDirectionMap<EConnectionStatus> IncomingStatuses;
				FDirectionMap<EConnectionStatus> OutgoingStatuses;

				TSubclassOf<ULogicalPlatformComponent> ComponentClass = ActiveMouseItem->GetItem()->GetType()->GetPlatformComponentClass(
					Platform->GetType(),
					ActiveMouseItem->GetItem()
				);
				ULogicalPlatformComponent *CDO = Cast<ULogicalPlatformComponent>(ComponentClass->GetDefaultObject(true));
				FDirectionFlagMap MyIncomingSupport = CDO->GetDefaultIncomingConnectionPorts();
				FDirectionFlagMap MyOutgoingSupport = CDO->GetDefaultOutgoingConnectionPorts();

				// Rotate to proper rotation
				EDirection DirIter = EDirection::EAST;
				while (DirIter != ActiveMouseItemDirection)
				{
					DirIter = RotateDirection(DirIter);
					MyIncomingSupport.Rotate();
					MyOutgoingSupport.Rotate();
				}

				for (EDirection Dir : TEnumRange<EDirection>())
				{
					EConnectionStatus Incoming;
					EConnectionStatus Outgoing;
					ULogicalPlatformComponent *Comp = Layout.GetDirection(Dir);

					bool bOutgoingPresent = MyOutgoingSupport.Get(Dir); // WE are trying to output in this direction
					bool bIncomingBlocked = !MyIncomingSupport.Get(Dir); // WE are not accepting input from them in this direction

					bool bOutgoingBlocked; // THEY are not accepting input from us in this direction
					bool bIncomingPresent; // THEY have output coming to us in this direction
					if (!Comp)
					{
						bOutgoingBlocked = false;
						bIncomingPresent = false;
					}
					else
					{
						FDirectionFlagMap TheirIncomingSupport = Comp->GetIncomingConnectionPorts();
						FDirectionFlagMap TheirOutgoingSupport = Comp->GetOutgoingConnectionPorts();

						bIncomingPresent = TheirOutgoingSupport.Get(OppositeDirection(Dir));
						bOutgoingBlocked = !TheirIncomingSupport.Get(OppositeDirection(Dir));
					}

					Outgoing = (!bOutgoingPresent ? EConnectionStatus::NO_CONNECTION : (bOutgoingBlocked ? EConnectionStatus::INVALID : EConnectionStatus::VALID));
					Incoming = (!bIncomingPresent ? EConnectionStatus::NO_CONNECTION : (bIncomingBlocked ? EConnectionStatus::INVALID : EConnectionStatus::VALID));

					IncomingStatuses.Set(Dir, Incoming);
					OutgoingStatuses.Set(Dir, Outgoing);
				}

				ActiveMouseItemActor->UpdateConnectionInfo(IncomingStatuses, OutgoingStatuses);
			}

			// Mark as valid if nothing's at the cell -- including the player :P
			FVector PlayerWorldPos = PlayerCharacter->GetActorLocation();
			FGridPosition PlayerGridPos = Platform->GetGridPosFromWorld(PlayerWorldPos.X, PlayerWorldPos.Y);
			if (!!Layout.Center || PlayerGridPos == GridPos)
			{
				bActiveMouseItemValid = false;
			}
			else
			{
				bActiveMouseItemValid = true;
			}
		}
		else
		{
			// Don't update actor location and definitely mark things as bad
			bActiveMouseItemValid = false;
		}

		// Regardless of location updates, rotate actor according to rotation
		FRotator Rotation = GetRotationFromDirection(this->ActiveMouseItemDirection);
		ActiveMouseItemActor->SetActorRotation(Rotation);

		// Update outlining info based on what we discovered as well
		const bool bValid = bActiveMouseItemValid;
		ActiveMouseItemActor->ForEachComponent<UPrimitiveComponent>(true, [bValid](UPrimitiveComponent *Component) {
			Component->SetRenderCustomDepth(true);
			Component->SetCustomDepthStencilValue(GetColorIndex(bValid ? EStandardColors::GREEN : EStandardColors::RED));
		});
	}
	else
	{
		bActiveMouseItemValid = false;
	}

	//bool bLastFrameActiveItemValid = bActiveMouseItemValid;
	//bActiveMouseItemValid = false;
	//if (ActiveMouseItemActor)
	//{
	//	if (PlayerCharacter && PlayerCharacter->GetShop())
	//	{
	//		AShop *ShopBuilding = PlayerCharacter->GetShop();
	//		UPlatform *PlatformBuilding = PlayerCharacter->GetPlatform();
	//		ULogicalShop *Shop = ShopBuilding->GetLogicalShop();
	//		ULogicalPlatform *Platform = PlatformBuilding->GetLogicalPlatform();

	//		if (Shop && Platform)
	//		{
	//			ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	//			bool bHit = false;
	//			if (LocalPlayer && LocalPlayer->ViewportClient)
	//			{
	//				FVector2D MousePosition;
	//				if (LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
	//				{
	//					FHitResult HitResult;
	//					FVector Origin;
	//					FVector BoxExtent;
	//					FCollisionQueryParams Params;

	//					Params.bTraceComplex = true;
	//					Params.AddIgnoredActor(ActiveMouseItemActor);

	//					if (GetHitResultAtScreenPosition(MousePosition, ECollisionChannel::ECC_Visibility, Params, HitResult)
	//						&& FMath::Abs(HitResult.Location.Z - PlatformBuilding->GetComponentLocation().Z) < 10)
	//					{
	//						// Query platform for grid position and what may already be there
	//						FGridPosition GridPos = Platform->GetGridPosFromWorld(HitResult.Location.X, HitResult.Location.Y);
	//						FVector WorldCenterPos = Platform->GetWorldPosFromGrid(GridPos, true);

	//						// Adjust hitpos to center of cell
	//						HitResult.Location.X = WorldCenterPos.X;
	//						HitResult.Location.Y = WorldCenterPos.Y;
	//						HitResult.Location.Z = PlatformBuilding->GetComponentLocation().Z;

	//						ActiveMouseItemActor->SetActorLocation(HitResult.Location);

	//						/*
	//						// Make sure player isn't in that cell
	//						FVector PlayerWorldPos = PlayerCharacter->GetActorLocation();
	//						FGridPosition PlayerGridPos = Platform->GetGridPosFromWorld(PlayerWorldPos.X, PlayerWorldPos.Y);
	//						if (PlayerGridPos == GridPos)
	//						{

	//							bActiveMouseItemCacheDirty = true; // Re-eval logic bits once we've moved
	//						}
	//						
	//						*/

	//						// Don't re-update if this is same position as last frame
	//						if (bActiveMouseItemCacheDirty 
	//							|| (GridPos != ActiveMouseItemCachePos || ActiveMouseItemDirection != ActiveMouseItemCacheDirection))
	//						{
	//							FLocalLayout Layout = Platform->GetComponent(GridPos);
	//							ActiveMouseItemCachePos = GridPos;
	//							ActiveMouseItemCacheDirection = ActiveMouseItemDirection;
	//							bActiveMouseItemCacheDirty = false;

	//							// For now, treat as valid if there's nothing there already
	//							bActiveMouseItemValid = !Layout.Center;

	//							// Build map of nearby connection opportunities and update actor
	//							FDirectionMap<EConnectionStatus> IncomingStatuses;
	//							FDirectionMap<EConnectionStatus> OutgoingStatuses;

	//							if (bActiveMouseItemValid)
	//							{
	//								TSubclassOf<ULogicalPlatformComponent> ComponentClass = ActiveMouseItem->GetItem()->GetType()->GetPlatformComponentClass(
	//									Platform->GetType(),
	//									ActiveMouseItem->GetItem()
	//								);
	//								ULogicalPlatformComponent *CDO = Cast<ULogicalPlatformComponent>(ComponentClass->GetDefaultObject(true));
	//								FDirectionFlagMap MyIncomingSupport = CDO->GetDefaultIncomingConnectionPorts();
	//								FDirectionFlagMap MyOutgoingSupport = CDO->GetDefaultOutgoingConnectionPorts();

	//								// Rotate to proper rotation
	//								EDirection DirIter = EDirection::EAST;
	//								while (DirIter != ActiveMouseItemDirection)
	//								{
	//									DirIter = RotateDirection(DirIter);
	//									MyIncomingSupport.Rotate();
	//									MyOutgoingSupport.Rotate();
	//								}

	//								for (EDirection Dir : TEnumRange<EDirection>())
	//								{
	//									EConnectionStatus Incoming;
	//									EConnectionStatus Outgoing;
	//									ULogicalPlatformComponent *Comp = Layout.GetDirection(Dir);

	//									// Cases?
	//									// If no comp in that slot, there won't be any incoming. Outgoing is unblocked or null.
	//									// If there is one, incoming exists if they push. Accept depends on support map.
	//									//     ->			outgoing exists if we push. Accept depends on if THEY support it.

	//									
	//									bool bOutgoingPresent = MyOutgoingSupport.Get(Dir); // WE are trying to output in this direction
	//									bool bIncomingBlocked = !MyIncomingSupport.Get(Dir); // WE are not accepting input from them in this direction

	//									bool bOutgoingBlocked; // THEY are not accepting input from us in this direction
	//									bool bIncomingPresent; // THEY have output coming to us in this direction
	//									if (!Comp)
	//									{
	//										bOutgoingBlocked = false;
	//										bIncomingPresent = false;
	//									}
	//									else
	//									{
	//										FDirectionFlagMap TheirIncomingSupport = Comp->GetIncomingConnectionPorts();
	//										FDirectionFlagMap TheirOutgoingSupport = Comp->GetOutgoingConnectionPorts();

	//										bIncomingPresent = TheirOutgoingSupport.Get(OppositeDirection(Dir));
	//										bOutgoingBlocked = !TheirIncomingSupport.Get(OppositeDirection(Dir));
	//									}

	//									Outgoing = (!bOutgoingPresent ? EConnectionStatus::NO_CONNECTION : (bOutgoingBlocked ? EConnectionStatus::INVALID : EConnectionStatus::VALID));
	//									Incoming = (!bIncomingPresent ? EConnectionStatus::NO_CONNECTION : (bIncomingBlocked ? EConnectionStatus::INVALID : EConnectionStatus::VALID));

	//									IncomingStatuses.Set(Dir, Incoming);
	//									OutgoingStatuses.Set(Dir, Outgoing);
	//								}
	//							}
	//							else
	//							{
	//								IncomingStatuses = FDirectionMap<EConnectionStatus>(EConnectionStatus::NO_CONNECTION);
	//								OutgoingStatuses = FDirectionMap<EConnectionStatus>(EConnectionStatus::NO_CONNECTION);
	//							}

	//							ActiveMouseItemActor->UpdateConnectionInfo(IncomingStatuses, OutgoingStatuses);
	//						}
	//						else
	//						{
	//							// Put back old valid flag
	//							// This instead of adding else to all cases to set it false
	//							bActiveMouseItemValid = bLastFrameActiveItemValid;
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}

	//	const bool bValid = bActiveMouseItemValid;
	//	ActiveMouseItemActor->ForEachComponent<UPrimitiveComponent>(true, [bValid](UPrimitiveComponent *Component) {
	//		Component->SetRenderCustomDepth(true);
	//		Component->SetCustomDepthStencilValue(GetColorIndex(bValid ? EStandardColors::GREEN : EStandardColors::RED));
	//	});

	//	// Rotate according to rotation
	//	FRotator Rotation = GetRotationFromDirection(this->ActiveMouseItemDirection);
	//	ActiveMouseItemActor->SetActorRotation(Rotation);
	//}
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
	bActiveMouseItemCacheDirty = true;
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

		if (!this->ActiveMouseItem->IsValid() || !ITEM_EXISTS(ActiveMouseItem->GetItem()))
		{
			ClearActiveItem();
		}
	}
}

void AFactoryPlayerController::OpenScreenAt(UFactoryHUDWidget *Screen, FVector WorldPos)
{
	this->OpenScreen(Screen); // sets AutoCloseScreen
	AutoCloseScreenSourceLocation = WorldPos;
	bAutoCloseScreenUsesLocation = true;
}

void AFactoryPlayerController::OpenScreen(UFactoryHUDWidget *Screen)
{
	AutoCloseScreen = Screen;
	bAutoCloseScreenUsesLocation = false;
	this->GetHudManager()->SetScreen(Screen);
}