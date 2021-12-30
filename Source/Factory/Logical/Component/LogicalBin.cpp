#include "LogicalBin.h"

#include "Factory/GameEnums.h"
#include "Factory/FactoryLocale.h"
#include "Factory/Logical/Inventory/Item.h"
#include "Factory/Building/Platform/Platform.h"
#include "Factory/Building/Platform/Component/Bin.h"
#include "Factory/Character/FactoryPlayerController.h"


APlatformComponent *ULogicalBin::SpawnWorldComponentInternal(UPlatform *Platform)
{
	FVector SpawnLoc;
	MakeSpawnLocation(SpawnLoc);
	ABin *WorldComp = Platform->GetWorld()->SpawnActor<ABin>(SpawnLoc, GetRotationFromDirection(Direction), MakeSpawnParams());
	WorldComp->SetLogicalBin(this);
	WorldComp->RegisterPlatform(Platform);
	return WorldComp;
}

void ULogicalBin::RemoveFromPlatform(ULogicalPlatform *Platform)
{
	Super::RemoveFromPlatform(Platform);

	; // TODO: drop items?
}

bool ULogicalBin::CanAccept_Implementation(EDirection DirectionIn, const UItem *ItemIn)
{
	return this->ItemType == nullptr
		|| (ItemIn->GetType() == ItemType
			&& ItemIn->GetCount() + ItemCount <= GetMaxItemCount());
}

UItem *ULogicalBin::InsertItem_Implementation(EDirection DirectionIn, UItem *ItemIn)
{
	UItem *Leftover = ItemIn;
	if (CanAccept_Implementation(DirectionIn, ItemIn))
	{
		if (!ItemType)
		{
			ItemType = ItemIn->GetType();
			ItemCount = ItemIn->GetCount();
			Leftover = nullptr;
			RefreshDisplayItem();
		}
		else
		{
			int Room = GetMaxItemCount() - ItemCount;
			if (Room > 0)
			{
				if (Room >= ItemIn->GetCount())
				{
					this->ItemCount += ItemIn->GetCount();
					Leftover = nullptr;
				}
				else
				{
					this->ItemCount += Room;
					ItemIn->Split(Room);
					// leave Leftover as ItemIn to be cloned later
				}
				RefreshDisplayItem();
			}
			// else Leftover = ItemIn
		}
	}

	return Leftover;
}

void ULogicalBin::PeekItems_Implementation(TArray<UItem*> &ItemArray)
{
	ItemArray = this->DisplayMultiItemCache;
}

bool ULogicalBin::CanTake_Implementation(EDirection DirectionIn, const UItem *ItemDemandOpt)
{
	bool Success = true;

	if (!ItemType || ItemCount <= 0)
	{
		Success = false;
	}
	else if (ItemDemandOpt)
	{
		if (ItemType != ItemDemandOpt->GetType() || ItemCount < ItemDemandOpt->GetCount())
		{
			Success = false;
		}
	}

	return Success;
}

UItem *ULogicalBin::TakeItem_Implementation(EDirection DirectionIn, const UItem *ItemDemandOpt)
{
	UItem *Taken = nullptr;

	if (!!ItemType && ItemCount > 0)
	{
		int32 Amt = -1;
		if (ItemDemandOpt)
		{
			if (ItemType == ItemDemandOpt->GetType())
			{
				Amt = FMath::Min(ItemCount, ItemDemandOpt->GetCount());
			}
		}
		else
		{
			Amt = FMath::Min(ItemCount, ItemType->GetMaxStackSize());
		}

		if (Amt > 0)
		{
			Taken = UItem::MakeItemEx(GetWorld(), ItemType);
			Taken->SetCount(Amt);

			this->ItemCount -= Amt;
			if (ItemCount <= 0)
			{
				ItemType = nullptr;
			}
			RefreshDisplayItem();
		}
		
	}

	return Taken;
}

EDataValidationResult ULogicalBin::IsDataValid(TArray<FText> & ValidationErrors)
{
	EDataValidationResult result = ULogicalPlatformComponent::IsDataValid(ValidationErrors);
	bool bSuccess = true;

	if (this->MaxStackCount <= 0)
	{
		ValidationErrors.Add(LOCTEXT("LogicalBinFail.MaxStackMissing", "MaxStackCount must be greater than 0"));
		bSuccess = false;
	}
	if (!this->BinMesh)
	{
		ValidationErrors.Add(LOCTEXT("LogicalBinFail.BinMeshMissing", "A Mesh for the bin mut be specified"));
		bSuccess = false;
	}

	if (!bSuccess && result != EDataValidationResult::Invalid)
	{
		result = EDataValidationResult::Invalid;
	}

	return result;
}

void ULogicalBin::RefreshDisplayItem()
{
	if (DisplaySingleItemCache
		&& DisplaySingleItemCache->GetType() == ItemType
		&& DisplaySingleItemCache->GetCount() == this->GetItemCount()
		) {
		return;
	}

	DisplayMultiItemCache.Empty();
	if (ItemType)
	{
		if (!DisplaySingleItemCache || DisplaySingleItemCache->GetType() != ItemType)
		{
			DisplaySingleItemCache = UItem::MakeItemEx(this, ItemType);
		}
		DisplaySingleItemCache->SetCount(GetItemCount());

		int32 Count = GetItemCount();
		while (Count > 0)
		{
			int32 Amt = FMath::Min(Count, ItemType->GetMaxStackSize());

			UItem *Item = UItem::MakeItemEx(GetWorld(), ItemType);
			Item->SetCount(Amt);
			DisplayMultiItemCache.Add(Item);

			Count -= Amt;
		}
	}
	else
	{
		DisplaySingleItemCache = nullptr;
	}

}

TArray<UItem*> ULogicalBin::GetItems_Implementation()
{
	return DisplayMultiItemCache;
}

bool ULogicalBin::CanFit_Implementation(const UItem *Item)
{
	if (!Item) return true;

	return
		(!ItemType || ItemType == Item->GetType()) // Check type is okay
		&&
		(GetItemCount() + Item->GetCount() <= Item->GetType()->GetMaxStackSize() * this->GetMaxStackCount()); // Check count fits
}

UItem *ULogicalBin::AddItem_Implementation(UItem *ItemIn)
{
	UItem *Leftover = ItemIn;

	if (!ItemType || ItemType == ItemIn->GetType())
	{
		int32 Room = GetMaxItemCount() - GetItemCount();
		if (Room > 0)
		{
			int32 Amt = FMath::Min(Room, ItemIn->GetCount());
			this->ItemCount += Amt;
			ItemIn->AddCount(-Amt);
			RefreshDisplayItem();
		}
	}
	
	return Leftover;
}

bool ULogicalBin::HasItem_Implementation(const UItem *Item)
{
	if (!Item) return false;

	return
		(ItemType && ItemType == Item->GetType()) // Check type is okay
		&&
		(GetItemCount() <= Item->GetCount()); // Check count is enough
}

UItem *ULogicalBin::TakeItem_Implementation(UItem *Item)
{
	UItem *Taken = nullptr;
	if (ItemType && ItemType == Item->GetType())
	{
		int32 Amt = FMath::Min(Item->GetCount(), this->ItemCount);
		if (Amt > 0)
		{
			Taken = UItem::MakeItemEx(GetWorld(), ItemType);
			Taken->SetCount(Amt);
			this->ItemCount -= Amt;
			if (ItemCount <= 0)
			{
				ItemType = nullptr;
			}
			RefreshDisplayItem();
		}
	}
	return Taken;
}

int32 ULogicalBin::GetMaxSlots_Implementation()
{
	return this->GetMaxStackCount();
}

UItem *ULogicalBin::TakeItemSlot_Implementation(int32 SlotIdx, int32 Count)
{
	UItem *Taken = nullptr;

	// Cheat and use display items (via GetItemSlot_Implementation) to figure
	// out what's "in the slot" and then subtract that
	UItem *Template = GetItemSlot_Implementation(SlotIdx);
	if (Template)
	{
		Taken = Template->Clone();
		if (Count > ItemCount)
		{
			Count = ItemCount;
		}

		Taken->SetCount(Count);
		this->ItemCount -= Taken->GetCount();
		if (ItemCount <= 0)
		{
			ItemType = nullptr;
		}
		RefreshDisplayItem();
	}

	return Taken;
}

int32 ULogicalBin::AddItemCountSlot_Implementation(int32 SlotIdx, int32 Count)
{
	if (ItemType)
	{
		int32 Room = this->GetMaxItemCount() - this->GetItemCount();
		int32 Amt = FMath::Min(Room, Count);

		this->ItemCount += Amt;
		Count -= Amt;
		RefreshDisplayItem();
	}

	return Count;
}

UItem *ULogicalBin::AddItemSlot_Implementation(int32 SlotIdx, UItem *ItemIn)
{
	// Basically treat as generic 'insert' since our slots are faked
	return Execute_AddItem(this, ItemIn);
}

UItem *ULogicalBin::GetItemSlot_Implementation(int32 SlotIdx)
{
	UItem *Item = nullptr;

	if (ItemType && ItemCount > 0)
	{
		// Take advantage of the fact that we make display items that happen
		// to line up with a sparse inventory array
		Item = (DisplayMultiItemCache.Num() > SlotIdx ? DisplayMultiItemCache[SlotIdx] : nullptr);
	}

	return Item;
}
