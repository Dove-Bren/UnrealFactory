#include "Inventory.h"

UInventory::UInventory()
{

}

UInventory::~UInventory()
{

}

bool UInventory::CanFit_Implementation(const UItem *Item)
{
	if (HeldItems.Num() < this->Execute_GetMaxSlots(this))
	{
		// If we haven't even filled out all the slots, we can definitely fit a new itemstack
		return true;
	}

	// See if we could merge the item in
	int Count = Item->GetCount();
	for (UItem *InSlot : HeldItems)
	{
		if (InSlot->MatchesType(Item))
		{
			Count -= (InSlot->GetMaxCount() - InSlot->GetCount());
			if (Count <= 0)
			{
				break;
			}
		}
	}

	return Count <= 0;
}

UItem *UInventory::AddItem_Implementation(UItem *ItemIn)
{
	// First, attempt to merge
	for (UItem *InSlot : HeldItems)
	{
		ItemIn = InSlot->Merge(ItemIn);

		if (!ITEM_EXISTS(ItemIn))
		{
			break;
		}
	}

	// Then handle any leftover amount
	if (ITEM_EXISTS(ItemIn))
	{
		// Add into new slot if a slot is available
		if (HeldItems.Num() < this->Execute_GetMaxSlots(this))
		{
			HeldItems.Add(ItemIn);
			ItemIn = nullptr;
		}
	}

	return ItemIn; // Whatever didn't fit
}

bool UInventory::HasItem_Implementation(const UItem *Item)
{
	int32 Count = Item->GetCount();
	for (UItem *InSlot : HeldItems)
	{
		if (InSlot->MatchesType(Item))
		{
			Count -= InSlot->GetCount();
			if (Count <= 0)
			{
				break;
			}
		}
	}
	return Count <= 0;
}

UItem *UInventory::TakeItem_Implementation(UItem *Item)
{
	UItem *Out = Item->Clone();
	Out->SetCount(0);

	// Walk backwards and take from the end first
	for (int i = HeldItems.Num() - 1; i >= 0 && Out->GetCount() < Item->GetCount(); i--)
	{
		UItem *InSlot = HeldItems[i];
		if (InSlot->MatchesType(Item))
		{
			// Conveniently calc for readibility
			int32 Wanted = Item->GetCount() - Out->GetCount();
			if (InSlot->GetCount() > Wanted)
			{
				// Slot has all the count we need
				Out->Merge(InSlot->Split(Wanted));
			}
			else
			{
				// We're going to be consuming all of a stack here
				HeldItems.RemoveAtSwap(i);
				Out->Merge(InSlot);
			}
		}
	}

	return Out->IsEmpty() ? nullptr : Out;
}

UItem *UInventory::GetItemSlot_Implementation(int32 SlotIdx)
{
	UItem *Item = nullptr;
	if (SlotIdx < this->HeldItems.Num())
	{
		Item = HeldItems[SlotIdx];
	}
	return Item;
}

UItem *UInventory::TakeItemSlot_Implementation(int32 SlotIdx, int32 Count)
{
	UItem *Item = nullptr;
	if (SlotIdx < this->HeldItems.Num() && Count)
	{
		Item = this->HeldItems[SlotIdx]->Split(Count);

		if (this->HeldItems[SlotIdx]->IsEmpty())
		{
			HeldItems.RemoveAtSwap(SlotIdx);
		}
	}
	return Item;
}

int32 UInventory::AddItemCountSlot_Implementation(int32 SlotIdx, int32 Count)
{
	int32 Leftover = Count;
	if (SlotIdx < this->HeldItems.Num())
	{
		Leftover = HeldItems[SlotIdx]->AddCount(Count);
	}
	return Leftover;
}

UItem *UInventory::AddItemSlot_Implementation(int32 SlotIdx, UItem *OtherItem)
{
	UItem *Leftover = OtherItem;
	if (SlotIdx < this->HeldItems.Num())
	{
		Leftover = HeldItems[SlotIdx]->Merge(OtherItem);
	}
	return Leftover;
}

UItem *UInventorySlotRef::RemoveItems(int32 Count)
{
	UItem *Taken = nullptr;
	if (this->IsValid())
	{
		const UItem *Ref = GetItem(); // For checking if stack was exhausted

		Taken = Inventory->Execute_TakeItemSlot(Inventory.GetObject(), SlotIdx, Count);

		if (GetItem() != Ref) // Slot Item changed!
		{
			// Assume we're invalid
			//this->SlotIdx = -1;
		}
	}
	return Taken;
}