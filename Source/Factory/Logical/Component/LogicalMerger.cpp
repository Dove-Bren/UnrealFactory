#include "LogicalMerger.h"

#include "Factory/GameEnums.h"
#include "Factory/Building/Platform/Platform.h"
#include "Factory/Building/Platform/Component/Merger.h"

ULogicalMerger::ULogicalMerger() : ULogicalPlatformComponent()
{
	;
}

APlatformComponent *ULogicalMerger::SpawnWorldComponentInternal(UPlatform *Platform)
{
	FVector SpawnLoc;
	MakeSpawnLocation(SpawnLoc);
	AMerger *WorldComp = Platform->GetWorld()->SpawnActor<AMerger>(SpawnLoc, GetRotationFromDirection(Direction), MakeSpawnParams());
	WorldComp->SetLogicalMerger(this);
	WorldComp->RegisterPlatform(Platform);
	return WorldComp;
}

void ULogicalMerger::RegisterPlatform(ULogicalPlatform *Platform, FGridPosition GridPosition)
{
	Super::RegisterPlatform(Platform, GridPosition);
}

void ULogicalMerger::RemoveFromPlatform(ULogicalPlatform *Platform)
{
	Super::RemoveFromPlatform(Platform);

	;
}

bool ULogicalMerger::IsActiveDuring(EGamePhase Phase)
{
	return true;
}

void ULogicalMerger::PullFromProducers()
{
	if (NextIncomeDirection == EDirection::MAX)
	{
		NextIncomeDirection = GetDirection();
	}

	if (!ITEM_EXISTS(this->Item))
	{
		// Do up to one full spin
		for (int i = 0; i < 4; i++)
		{
			int turns = 0;
			EDirection DirCursor = GetDirection();
			while (DirCursor != NextIncomeDirection) { turns++; DirCursor = RotateDirection(DirCursor); }
			IItemHandler *Handler = nullptr;

			switch (turns)
			{
			case 0: // Next 'income' direction is one we're facing
			default:
				Handler = nullptr;
				break;
			case 3: // Left
				Handler = CachedLeftProducer;
				break;
			case 1: // Next 'income' direction is right
				Handler = CachedRightProducer;
				break;
			case 2: // Next 'income' direction is behind us
				Handler = CachedRearProducer;
				break;
			}

			if (Handler != nullptr)
			{
				SetItem(IItemHandler::AttemptTake(Handler, OppositeDirection(NextIncomeDirection), nullptr), NextIncomeDirection);
			}

			if (ITEM_EXISTS(this->Item))
			{
				break; // Success.
			}
			else
			{
				// Rotate NextIncomeDirection manually
				NextIncomeDirection = RotateDirection(NextIncomeDirection);
			}
		}

	}
}

void ULogicalMerger::ShopTick(EGamePhase Phase)
{
	if (ITEM_EXISTS(this->Item)
		&& LastIncomeTick != UFactorySingletons::GetInstance(GetWorld())->TotalTickCount
		&& CachedReceiver
		)
	{
		SetItem(IItemHandler::AttemptInsert(CachedReceiver, OppositeDirection(Direction), Item));
	}

	// If (after previousprocessing) we have no items, pull from any producers behind us.
	// This makes same-frame back propogation work
	PullFromProducers();

	// If we still don't have an item, rotate out next income direction for next time
	if (!ITEM_EXISTS(this->Item))
	{
		NextIncomeDirection = RotateDirection(NextIncomeDirection);
	}
}

bool ULogicalMerger::RefreshNearby(FLocalLayout NearbyLayout)
{
	bool bChanged = ULogicalPlatformComponent::RefreshNearby(NearbyLayout);
	FixupLocalLayout(NearbyLayout);

	IItemHandler *StartReceiver = CachedReceiver;
	IItemHandler *StartRearProducer = CachedRearProducer;
	IItemHandler *StartRightProducer = CachedRightProducer;
	IItemHandler *StartLeftProducer = CachedLeftProducer;

	// Look to see if there's a machine to feed into
	ULogicalPlatformComponent *Comp = NearbyLayout.GetDirection(GetDirection());
	if (!Comp)
	{
		CachedReceiver = nullptr;
	}
	else
	{
		IItemHandler *Handler = Cast<IItemHandler>(Comp);

		if (Comp->GetIncomingConnectionPorts().Get(OppositeDirection(GetDirection()))
			&& Handler)
		{
			CachedReceiver = Handler;
		}
		else
		{
			CachedReceiver = nullptr;
		}
	}

	// Repeat but looking for receivers
	EDirection LookDirection = GetDirection();
	IItemHandler** Handlers[] = {
		&CachedRightProducer,
		&CachedRearProducer,
		&CachedLeftProducer,
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

			if (Handler && Comp->GetOutgoingConnectionPorts().Get(OppositeDirection(LookDirection)))
			{
				*(Handlers[i]) = Handler;
			}
			else
			{
				*(Handlers[i]) = nullptr;
			}
		}
	}

	return bChanged
		|| (CachedReceiver != StartReceiver)
		|| (CachedRearProducer != StartRearProducer)
		|| (CachedRightProducer != StartRightProducer)
		|| (CachedLeftProducer != StartLeftProducer);
}

bool ULogicalMerger::CanAccept_Implementation(EDirection DirectionIn, const UItem *ItemIn)
{
	return false; // Only move items in shop tick
	/*return DirectionIn != this->Direction
		&& DirectionIn == NextIncomeDirection
		&& (!ITEM_EXISTS(this->Item));*/
}

UItem *ULogicalMerger::InsertItem_Implementation(EDirection DirectionIn, UItem *ItemIn)
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

void ULogicalMerger::PeekItems_Implementation(TArray<UItem*> &ItemArray)
{
	if (ITEM_EXISTS(Item))
	{
		ItemArray.Add(Item);
	}
}

bool ULogicalMerger::CanTake_Implementation(EDirection DirectionIn, const UItem *ItemDemandOpt)
{
	return false; // Only accept items in shop tick
	/*bool Success = true;

	if (DirectionIn != Direction)
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

UItem *ULogicalMerger::TakeItem_Implementation(EDirection DirectionIn, const UItem *ItemDemandOpt)
{
	UItem *Taken = nullptr;

	if (DirectionIn == Direction && Item)
	{
		if (ItemDemandOpt)
		{
			if (Item->MatchesType(ItemDemandOpt))
			{
				Taken = Item->Split(ItemDemandOpt->GetCount());
				if (Item->IsEmpty())
				{
					ClearItem();
				}
			}
		}
		else
		{
			Taken = Item;
			ClearItem();

			// Backwards propogation on belts to avoid frame-order lag?
			PullFromProducers();
		}
	}

	return Taken;
}

void ULogicalMerger::SetItem(UItem *NewItem, EDirection FromDirection)
{
	if (!UItem::ItemsEqual(Item, NewItem))
	{
		this->LastIncomeTick = UFactorySingletons::GetInstance(GetWorld())->TotalTickCount;
	}

	if (FromDirection != EDirection::MAX)
	{
		this->NextIncomeDirection = RotateDirection(FromDirection);
	}

	this->Item = NewItem;
}

void ULogicalMerger::ClearItem()
{
	this->Item = nullptr;
	//this->NextIncomeDirection = EDirection::MAX;
}