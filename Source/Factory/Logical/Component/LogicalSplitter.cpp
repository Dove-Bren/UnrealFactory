#include "LogicalSplitter.h"

#include "Factory/GameEnums.h"
#include "Factory/Building/Platform/Platform.h"
#include "Factory/Building/Platform/Component/Splitter.h"

ULogicalSplitter::ULogicalSplitter() : ULogicalPlatformComponent()
{
	;
}

APlatformComponent *ULogicalSplitter::SpawnWorldComponentInternal(UPlatform *Platform)
{
	FVector SpawnLoc;
	MakeSpawnLocation(SpawnLoc);
	ASplitter *WorldComp = Platform->GetWorld()->SpawnActor<ASplitter>(SpawnLoc, GetRotationFromDirection(Direction), MakeSpawnParams());
	WorldComp->SetLogicalSplitter(this);
	WorldComp->RegisterPlatform(Platform);
	return WorldComp;
}

void ULogicalSplitter::RegisterPlatform(ULogicalPlatform *Platform, FGridPosition GridPosition)
{
	Super::RegisterPlatform(Platform, GridPosition);
}

void ULogicalSplitter::RemoveFromPlatform(ULogicalPlatform *Platform)
{
	Super::RemoveFromPlatform(Platform);

	;
}

bool ULogicalSplitter::IsActiveDuring(EGamePhase Phase)
{
	return true;
}

void ULogicalSplitter::PushToReceivers()
{
	if (NextOutputDirection == EDirection::MAX)
	{
		NextOutputDirection = GetDirection();
	}

	if (ITEM_EXISTS(this->Item))
	{
		// Do up to one full spin
		for (int i = 0; i < 4; i++)
		{
			int turns = 0;
			EDirection DirCursor = GetDirection();
			while (DirCursor != NextOutputDirection) { turns++; DirCursor = RotateDirection(DirCursor); }
			IItemHandler *Handler = nullptr;

			switch (turns)
			{
			case 2: // Next 'output' direction is one we take input from
			default:
				Handler = nullptr;
				break;
			case 1: // Right
				Handler = CachedRightReceiver;
				break;
			case 3: // Next 'output' direction is left
				Handler = CachedLeftReceiver;
				break;
			case 0: // Next 'output' direction is ahead of us
				Handler = CachedForwardReceiver;
				break;
			}

			if (Handler != nullptr)
			{
				SetItem(IItemHandler::AttemptInsert(Handler, OppositeDirection(NextOutputDirection), Item), NextOutputDirection);
			}

			// Rotate NextIncomeDirection
			// Either sets up for next loop or for next time we actually output if we were able to push our stuff
			NextOutputDirection = RotateDirection(NextOutputDirection);
			if (!ITEM_EXISTS(this->Item))
			{
				break; // Success.
			}
		}
	}
}

void ULogicalSplitter::ShopTick(EGamePhase Phase)
{
	if (ITEM_EXISTS(this->Item)
		&& LastIncomeTick != UFactorySingletons::GetInstance(GetWorld())->TotalTickCount)
	{
		PushToReceivers();
	}

	// If (after previousprocessing) we have no items, pull from any producers behind us.
	// This makes same-frame back propogation work
	if (!ITEM_EXISTS(this->Item) && !!CachedRearProducer)
	{
		SetItem(IItemHandler::AttemptTake(CachedRearProducer, Direction, nullptr), OppositeDirection(Direction));
	}

	// If we still don't have an item, rotate out next output direction for next time
	/*if (!ITEM_EXISTS(this->Item))
	{
		NextOutputDirection = RotateDirection(NextOutputDirection);
	}*/
}

bool ULogicalSplitter::RefreshNearby(FLocalLayout NearbyLayout)
{
	bool bChanged = ULogicalPlatformComponent::RefreshNearby(NearbyLayout);
	FixupLocalLayout(NearbyLayout);

	IItemHandler *StartForwardReceiver = CachedForwardReceiver;
	IItemHandler *StartRightReceiver = CachedRightReceiver;
	IItemHandler *StartLeftReceiver = CachedLeftReceiver;
	IItemHandler *StartProducer = CachedRearProducer;

	ULogicalPlatformComponent *Comp = NearbyLayout.GetDirection(GetDirection());

	// Look to see if there's a machine to feed into
	EDirection LookDirection = OppositeDirection(GetDirection());
	IItemHandler** Handlers[] = {
		&CachedLeftReceiver,
		&CachedForwardReceiver,
		&CachedRightReceiver,
	};
	for (int i = 0; i < 3; i++)
	{
		LookDirection = RotateDirection(LookDirection);
		Comp = NearbyLayout.GetDirection(LookDirection);
		if (!Comp)
		{
			*(Handlers[i]) = nullptr;
		}
		else
		{
			IItemHandler *Handler = Cast<IItemHandler>(Comp);

			if (Comp->GetIncomingConnectionPorts().Get(OppositeDirection(GetDirection()))
				&& Handler)
			{
				*(Handlers[i]) = Handler;
			}
			else
			{
				*(Handlers[i]) = nullptr;
			}
		}
	}

	// Repeat but looking at behind us
	Comp = NearbyLayout.GetDirection(OppositeDirection(GetDirection()));
	if (!Comp)
	{
		CachedRearProducer = nullptr;
	}
	else
	{
		IItemHandler *Handler = Cast<IItemHandler>(Comp);

		if (Handler && Comp->GetOutgoingConnectionPorts().Get(GetDirection()))
		{
			CachedRearProducer = Handler;
		}
		else
		{
			CachedRearProducer = nullptr;
		}
	}

	return bChanged
		|| (CachedLeftReceiver != StartLeftReceiver)
		|| (CachedForwardReceiver != StartForwardReceiver)
		|| (CachedRightReceiver != StartRightReceiver)
		|| (CachedRearProducer != StartProducer);
}

bool ULogicalSplitter::CanAccept_Implementation(EDirection DirectionIn, const UItem *ItemIn)
{
	return false; // Only accept items in shop tick
	//return DirectionIn == OppositeDirection(this->Direction) && (!ITEM_EXISTS(this->Item));
}

UItem *ULogicalSplitter::InsertItem_Implementation(EDirection DirectionIn, UItem *ItemIn)
{
	UItem *Leftover = ItemIn;
	if (CanAccept_Implementation(DirectionIn, ItemIn))
	{
		if (!ITEM_EXISTS(Item))
		{
			SetItem(ItemIn, DirectionIn);
			Leftover = nullptr;
		}
		else
		{
			Leftover = Item->Merge(ItemIn);
		}
	}

	return Leftover;
}

void ULogicalSplitter::PeekItems_Implementation(TArray<UItem*> &ItemArray)
{
	if (ITEM_EXISTS(Item))
	{
		ItemArray.Add(Item);
	}
}

bool ULogicalSplitter::CanTake_Implementation(EDirection DirectionIn, const UItem *ItemDemandOpt)
{
	return false; // Only push items in shop tick
	/*bool Success = true;

	if (DirectionIn != NextOutputDirection)
	{
		Success = false;
	}
	else if (!ITEM_EXISTS(Item))
	{
		Success = false;
	}
	else if (ItemDemandOpt)
	{
		if (!Item->MatchesType(ItemDemandOpt) || Item->GetCount() < ItemDemandOpt->GetCount())
		{
			Success = false;
		}
	}

	return Success;*/
}

UItem *ULogicalSplitter::TakeItem_Implementation(EDirection DirectionIn, const UItem *ItemDemandOpt)
{
	UItem *Taken = nullptr;

	if (DirectionIn == NextOutputDirection && Item)
	{
		if (ItemDemandOpt)
		{
			if (Item->MatchesType(ItemDemandOpt))
			{
				Taken = Item->Split(ItemDemandOpt->GetCount());
				if (Item->IsEmpty())
				{
					//ClearItem();
					SetItem(nullptr, DirectionIn);
				}
			}
		}
		else
		{
			Taken = Item;
			ClearItem();

			// Backwards propogation on belts to avoid frame-order lag?
			if (!UItem::ItemExists(Item) && !!CachedRearProducer)
			{
				SetItem(IItemHandler::AttemptTake(CachedRearProducer, Direction, nullptr), OppositeDirection(Direction));
			}
		}
	}

	return Taken;
}

void ULogicalSplitter::SetItem(UItem *NewItem, EDirection FromDirection)
{
	if (!UItem::ItemsEqual(Item, NewItem))
	{
		this->LastIncomeTick = UFactorySingletons::GetInstance(GetWorld())->TotalTickCount;
	}

	this->Item = NewItem;
}

void ULogicalSplitter::ClearItem()
{
	this->Item = nullptr;
	//this->NextOutputDirection = EDirection::MAX;
}