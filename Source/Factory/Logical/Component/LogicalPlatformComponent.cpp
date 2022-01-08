#include "LogicalPlatformComponent.h"

#include "Factory/Character/FactoryPlayerController.h"
#include "Factory/Logical/LogicalPlatform.h"
//#include "Factory/UI/Popup/SelectionPopup.h"

#include "Factory/Building/Platform/Component/PlatformComponent.h"

ULogicalPlatformComponent::ULogicalPlatformComponent()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FClassFinder<UPopupMenuWidget> PopupWidget;
		FConstructorStatics()
			: PopupWidget(TEXT("WidgetBlueprint'/Game/Factory/UI/PopupMenu/PopupSelectionMenu'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	if (ConstructorStatics.PopupWidget.Succeeded())
	{
		this->PopupWidgetClass = ConstructorStatics.PopupWidget.Class;
	}

	bRemoveable = true;
	//bool bUsable;
	//bool bMoveable;
	bRotatable = true;
	HighlightColor = EStandardColors::ORANGE;
}

void ULogicalPlatformComponent::RegisterPlatform(ULogicalPlatform * Platform, FGridPosition GridPosition)
{
	this->ParentPlatform = Platform;
	this->Position = GridPosition;
}

void ULogicalPlatformComponent::RemoveFromPlatform(ULogicalPlatform *Platform)
{
	this->ParentPlatform = nullptr;
}

void ULogicalPlatformComponent::StartPhase(EGamePhase Phase)
{
	;
}

bool ULogicalPlatformComponent::IsActiveDuring(EGamePhase Phase)
{
	return true;
}

void ULogicalPlatformComponent::ShopTick(EGamePhase Phase)
{
	;
}

bool ULogicalPlatformComponent::RefreshConnections()
{
	// TODO gather nearby layout
	FLocalLayout Layout{};

	return RefreshNearby(FixupLocalLayout(Layout));
}

bool ULogicalPlatformComponent::RefreshNearby(FLocalLayout NearbyLayout)
{
	return false;
}

void ULogicalPlatformComponent::MakeSpawnLocation(FVector &Location)
{
	if (!ParentPlatform)
	{
		Location.X = Location.Y = Location.Z = 0;
	}
	else
	{
		Location = ParentPlatform->GetWorldPosFromGrid(Position, true);
	}
}

FLocalLayout & ULogicalPlatformComponent::FixupLocalLayout(FLocalLayout & Existing)
{
	if (!Existing.Center && ParentPlatform)
	{
		Existing = ParentPlatform->GetComponent(Position);
	}
	return Existing;
}

APlatformComponent *ULogicalPlatformComponent::GetWorldActor()
{
	if (WorldActor && !WorldActor->IsValidLowLevel())
	{
		WorldActor = nullptr;
	}
	return WorldActor;
}

void ULogicalPlatformComponent::RefreshWorldActor()
{
	APlatformComponent *Actor = GetWorldActor(); // Not using variable to give chance to clear if it's been destroyed
	if (Actor)
	{
		Actor->Refresh();
	}
}

FDirectionFlagMap ULogicalPlatformComponent::GetIncomingConnectionPorts()
{
	FDirectionFlagMap Map = this->GetDefaultIncomingConnectionPorts();
	EDirection DirIter = EDirection::EAST;
	while (DirIter != GetDirection())
	{
		DirIter = RotateDirection(DirIter);
		Map.Rotate();
	}
	return Map;
}

FDirectionFlagMap ULogicalPlatformComponent::GetOutgoingConnectionPorts()
{
	FDirectionFlagMap Map = this->GetDefaultOutgoingConnectionPorts();
	EDirection DirIter = EDirection::EAST;
	while (DirIter != GetDirection())
	{
		DirIter = RotateDirection(DirIter);
		Map.Rotate();
	}
	return Map;
}

APlatformComponent *ULogicalPlatformComponent::SpawnWorldComponent(UPlatform *Platform)
{
	APlatformComponent *Actor = this->SpawnWorldComponentInternal(Platform);
	this->WorldActor = Actor;
	return Actor;
}

bool ULogicalPlatformComponent::OnUse(APlayerCharacter *Player)
{
	return false;
}

bool ULogicalPlatformComponent::ShouldHighlight(APlayerCharacter *Player, float Distance)
{
	AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	return (bUsable /*|| bMoveable*/ || bRotatable || bRemoveable)
		&& (!Controller || !Controller->GetActiveMouseItem())
		;
}

EStandardColors ULogicalPlatformComponent::GetHighlightColor(APlayerCharacter *Player, float Distance)
{
	return (!Player || Player->GetMaxReach() >= Distance)
		? HighlightColor
		: EStandardColors::DARK_GRAY;
}

bool ULogicalPlatformComponent::GetClickOptions(APlayerCharacter *Player, float Distance, FVector ActorLocation, ClickOption **DefaultOptOut, TArray<ClickOption> *OptionsOut)
{
	*DefaultOptOut = nullptr;

	AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// Use (first cause this is likely what we want to default to)
	if (this->bUsable)
	{
		OptionsOut->Emplace(*FString::Printf(TEXT("Use")), [this, Controller, Player]() {
			if (!this->ParentPlatform)
			{
				return;
			}

			this->OnUse(Player);

			if (Controller)
			{
				Controller->GetHudManager()->SetPopup(nullptr); // Close popup on click
			}
		});
	}

	// Rotate
	if (this->bRotatable)
	{
		OptionsOut->Emplace(*FString::Printf(TEXT("Rotate")), [this, Controller, Player]() {
			if (!this->ParentPlatform)
			{
				return;
			}

			EDirection OldDirection = this->Direction;
			this->Direction = RotateDirection(this->Direction);
			this->ParentPlatform->RefreshAround(this->GetPosition());
			this->OnRotation(OldDirection, this->Direction);

			if (Controller)
			{
				Controller->GetHudManager()->SetPopup(nullptr); // Close popup on click
			}
		});
	}

	//// Move
	//if (this->bMoveable)
	//{
	//	OptionsOut->Emplace(*FString::Printf(TEXT("Move")), [this, Controller, Player]() {
	//		if (!this->ParentPlatform)
	//		{
	//			return;
	//		}

	//		; // Set as placing again?

	//		if (Controller)
	//		{
	//			Controller->GetHudManager()->SetPopup(nullptr); // Close popup on click
	//		}
	//	});
	//}

	// Remove
	if (this->bRemoveable)
	{
		FString NameString;
		if (!!this->RemoveItemType)
		{
			NameString = FString::Printf(TEXT("Pickup"));
		}
		else
		{
			NameString = FString::Printf(TEXT("Remove"));
		}

		OptionsOut->Emplace(*NameString, [this, Controller, Player]() {
			if (!this->ParentPlatform)
			{
				return;
			}

			// If return item is present, add to inventory
			bool bCanRemove = true;
			if (!!this->RemoveItemType)
			{
				if (nullptr != Player->GetInventory()->Execute_AddItem(Player->GetInventory(), UItem::MakeItem(Player, RemoveItemType)))
				{
					bCanRemove = false;
				}
			}

			// Remove from platform if still okay
			if (bCanRemove)
			{
				this->GetParentPlatform()->RemoveComponent(this);
				this->RemoveFromPlatform(GetParentPlatform()); // Before fetching parent platform
				if (WorldActor)
				{
					WorldActor->RemoveFromPlatform();
					WorldActor->GetWorld()->DestroyActor(WorldActor);
				}
			}
			
			if (Controller)
			{
				Controller->GetHudManager()->SetPopup(nullptr); // Close popup on click
			}
		});
	}

	if (OptionsOut->Num() > 0)
	{
		*DefaultOptOut = &((*OptionsOut)[0]);
	}

	return *DefaultOptOut != nullptr; // Same as checking if size of options is > 0
}

void ULogicalPlatformComponent::OnRotation(EDirection OldDirection, EDirection NewDirection)
{
	if (WorldActor)
	{
		WorldActor->SetActorRotation(GetRotationFromDirection(NewDirection));
	}
}