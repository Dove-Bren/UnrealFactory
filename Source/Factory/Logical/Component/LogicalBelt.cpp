#include "LogicalBelt.h"

#include "Factory/GameEnums.h"
#include "Factory/Building/Platform/Platform.h"
#include "Factory/Building/Platform/Component/Belt.h"
#include "Factory/Logical/Component/LogicalBin.h"

///*static*/ ULogicalBelt *ULogicalBelt::MakeBelt(EDirection Direction)
//{
//	ULogicalBelt *Belt = NewObject<ULogicalBelt>();
//	Belt->SetDirection(Direction);
//	return Belt;
//}

ULogicalBelt::ULogicalBelt() : ULogicalPlatformComponent()
{
	fSpeedTicks = 20;
	this->bMoveable = false;
}

APlatformComponent *ULogicalBelt::SpawnWorldComponentInternal(UPlatform *Platform)
{
	FVector SpawnLoc;
	MakeSpawnLocation(SpawnLoc);
	ABelt *WorldComp = Platform->GetWorld()->SpawnActor<ABelt>(SpawnLoc, GetRotationFromDirection(Direction), MakeSpawnParams());
	WorldComp->SetLogicalBelt(this);
	WorldComp->RegisterPlatform(Platform);
	return WorldComp;
}

void ULogicalBelt::RegisterPlatform(ULogicalPlatform *Platform, FGridPosition GridPosition)
{
	Super::RegisterPlatform(Platform, GridPosition);

	; // Maybe at some point we'll wnat to track all the belts specifically
}

void ULogicalBelt::RemoveFromPlatform(ULogicalPlatform *Platform)
{
	Super::RemoveFromPlatform(Platform);

	;
}

bool ULogicalBelt::IsActiveDuring(EGamePhase Phase)
{
	return true;
}

void ULogicalBelt::ShopTick(EGamePhase Phase)
{
	if (ITEM_EXISTS(this->Item) && fItemProgress < 1.0f && LastIncomeTick != UFactorySingletons::GetInstance(GetWorld())->TotalTickCount)
	{
		fItemProgress += GetProgressPerTick();
	}

	// maybe should use "CanTake" function to avoid code duplication?
	if (this->Item && fItemProgress >= 1.0f && CachedReceiver)
	{
		SetItem(IItemHandler::AttemptInsert(CachedReceiver, OppositeDirection(Direction), Item));
	}

	// If (after previousprocessing) we have no items, pull from any producers behind us.
	// This makes same-frame back propogation work
	if (!ITEM_EXISTS(this->Item) && !!CachedRearProducer)
	{
		SetItem(IItemHandler::AttemptTake(CachedRearProducer, Direction, nullptr), OppositeDirection(Direction));
	}
}

bool ULogicalBelt::RefreshNearby(FLocalLayout NearbyLayout)
{
	bool bChanged = ULogicalPlatformComponent::RefreshNearby(NearbyLayout);
	FixupLocalLayout(NearbyLayout);

	IItemHandler *StartReceiver = CachedReceiver;
	IItemHandler *StartProducer = CachedRearProducer;

	// Look to see if there's a machine to feed into
	ULogicalPlatformComponent *Comp = NearbyLayout.GetDirection(GetDirection());
	bCachedExtended = false;
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

			ULogicalBelt *OtherBelt = Cast<ULogicalBelt>(Comp);
			if (OtherBelt)
			{
				EDirection OtherDir = OtherBelt->GetDirection();
				bCachedExtended = (OtherDir != GetDirection() && OtherDir != OppositeDirection(GetDirection()));
			}
		}
		else
		{
			CachedReceiver = nullptr;
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

		// Note: We treat bins as 'producers' for belts even though they don't advertise as such

		if (Handler && 
			(!!Cast<ULogicalBin>(Comp) || Comp->GetOutgoingConnectionPorts().Get(GetDirection())))
		{
			CachedRearProducer = Handler;
		}
		else
		{
			CachedRearProducer = nullptr;
		}
	}

	return bChanged || (CachedReceiver != StartReceiver) || (CachedRearProducer != StartProducer);
}

bool ULogicalBelt::CanAccept_Implementation(EDirection DirectionIn, const UItem *ItemIn)
{
	return DirectionIn != this->Direction && (!ITEM_EXISTS(this->Item));
}

UItem *ULogicalBelt::InsertItem_Implementation(EDirection DirectionIn, UItem *ItemIn)
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

void ULogicalBelt::PeekItems_Implementation(TArray<UItem*> &ItemArray)
{
	if (ITEM_EXISTS(Item))
	{
		ItemArray.Add(Item);
	}
}

bool ULogicalBelt::CanTake_Implementation(EDirection DirectionIn, const UItem *ItemDemandOpt)
{
	bool Success = true;

	if (DirectionIn != Direction)
	{
		Success = false;
	}
	else if (!ITEM_EXISTS(Item) || fItemProgress < 1.0f)
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

	return Success;
}

UItem *ULogicalBelt::TakeItem_Implementation(EDirection DirectionIn, const UItem *ItemDemandOpt)
{
	UItem *Taken = nullptr;

	if (DirectionIn == Direction && Item && fItemProgress >= 1.0f)
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
			if (!UItem::ItemExists(Item) && !!CachedRearProducer)
			{
				SetItem(IItemHandler::AttemptTake(CachedRearProducer, Direction, nullptr), OppositeDirection(Direction));
			}
		}
	}

	return Taken;
}

void ULogicalBelt::SetItem(UItem *NewItem, EDirection FromDirection)
{
	if (!UItem::ItemsEqual(Item, NewItem))
	{
		this->fItemProgress = 0;
		this->LastIncomeTick = UFactorySingletons::GetInstance(GetWorld())->TotalTickCount;
	}

	if (FromDirection != EDirection::MAX)
	{
		this->LastIncomeDirection = FromDirection;
	}

	this->Item = NewItem;
}

void ULogicalBelt::ClearItem()
{
	this->Item = nullptr;
	this->fItemProgress = 0;
}

float ULogicalBelt::GetItemProgress(float PartialTicks)
{
	return FMath::Min(1.0f, 
		this->fItemProgress	+ (PartialTicks * GetProgressPerTick())
	);
}
