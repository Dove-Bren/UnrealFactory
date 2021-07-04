#include "LogicalBelt.h"

#include "Factory/Building/Platform/Platform.h"
#include "Factory/Building/Platform/Component/Belt.h"
#include "Factory/GameEnums.h"

///*static*/ ULogicalBelt *ULogicalBelt::MakeBelt(EDirection Direction)
//{
//	ULogicalBelt *Belt = NewObject<ULogicalBelt>();
//	Belt->SetDirection(Direction);
//	return Belt;
//}

APlatformComponent *ULogicalBelt::SpawnWorldComponent(UPlatform *Platform)
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
	; // TODO do belt mechs!
}

void ULogicalBelt::RefreshNearby(FLocalLayout NearbyLayout)
{
	; // TODO find nearby things and create connections!
}

IItemHandler *ULogicalBelt::GetReceivingHandler()
{
	return nullptr; // TODO get thing we're connected to and return it!
}

bool ULogicalBelt::CanAccept_Implementation(EDirection DirectionIn, const UItem *ItemIn)
{
	return DirectionIn != this->Direction
		&& (!ITEM_EXISTS(this->Item) || this->Item->CanMerge(ItemIn));
	// TODO rate limitting?
}

UItem *ULogicalBelt::InsertItem_Implementation(EDirection DirectionIn, UItem *ItemIn)
{
	UItem *Leftover = nullptr;
	if (CanAccept_Implementation(DirectionIn, ItemIn))
	{
		if (!ITEM_EXISTS(Item))
		{
			Item = ItemIn;
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
		ItemArray.Add(Item->Clone());
	}
}

bool ULogicalBelt::CanTake_Implementation(EDirection DirectionIn, const UItem *ItemDemandOpt)
{
	bool Success = true;

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

	return Success;
}

UItem *ULogicalBelt::TakeItem_Implementation(EDirection DirectionIn, const UItem *ItemDemandOpt)
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
					Item = nullptr;
				}
			}
		}
		else
		{
			Taken = Item;
			Item = nullptr;
		}
	}

	return Taken;
}