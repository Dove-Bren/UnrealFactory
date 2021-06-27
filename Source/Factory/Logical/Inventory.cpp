#include "Inventory.h"

UInventory::UInventory()
{

}

UInventory::~UInventory()
{

}

bool UInventory::CanFit(const UItem *Item)
{
	if (HeldItems.Num() < this->GetMaxSlots())
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

UItem *UInventory::AddItem(UItem *ItemIn)
{
	// First, attempt to merge
	for (UItem *InSlot : HeldItems)
	{
		ItemIn = InSlot->Merge(ItemIn);

		if (ItemIn == nullptr || ItemIn->IsEmpty())
		{
			break;
		}
	}

	// Then handle any leftover amount
	if (ItemIn && !ItemIn->IsEmpty())
	{
		// Add into new slot if a slot is available
		if (HeldItems.Num() < this->GetMaxSlots())
		{
			HeldItems.Add(ItemIn);
			ItemIn = nullptr;
		}
	}

	return ItemIn; // Whatever didn't fit
}

bool UInventory::HasItem(const UItem *Item)
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

UItem *UInventory::TakeItem(UItem *Item)
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
