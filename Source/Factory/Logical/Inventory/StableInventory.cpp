#include "StableInventory.h"

UStableInventory::UStableInventory() : UInventory()
{
	Slots.Init(nullptr, this->Execute_GetMaxSlots(this));
}

UStableInventory::~UStableInventory()
{

}

bool UStableInventory::CanFit_Implementation(const UItem *Item)
{
	// See if we could merge the item in
	int Count = Item->GetCount();
	for (UItem *InSlot : Slots)
	{
		if (!InSlot)
		{
			// Empty slot! Can definitely fit!
			return true;
		}

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

UItem *UStableInventory::AddItem_Implementation(UItem *ItemIn)
{
	// First, attempt to merge
	for (int i = 0; i < this->Execute_GetMaxSlots(this); i++)
	{
		UItem *InSlot = Slots[i];
		if (!InSlot)
		{
			Slots[i] = ItemIn->Clone();
			ItemIn = nullptr; // Stored it all
			break;
		}

		ItemIn = InSlot->Merge(ItemIn);

		if (!ITEM_EXISTS(ItemIn))
		{
			break;
		}
	}

	return ItemIn; // Whatever didn't fit

	//// First, attempt to merge (but track first empty if we find one)
	//int EmptySlot = -1;
	//for (int i = 0; i < this->Execute_GetMaxSlots(this); i++)
	//{
	//	UItem *InSlot = Slots[i];
	//	if (!InSlot)
	//	{
	//		if (EmptySlot < 0)
	//		{
	//			EmptySlot = i;
	//		}
	//		continue;
	//	}

	//	ItemIn = InSlot->Merge(ItemIn);

	//	if (!ITEM_EXISTS(ItemIn))
	//	{
	//		break;
	//	}
	//}

	//// If remaining, add to an empty slot if we found one
	//if (ITEM_EXISTS(ItemIn) && EmptySlot > 0)
	//{
	//	Slots[EmptySlot] = ItemIn->Clone();
	//	ItemIn = nullptr;
	//}

	//return ItemIn; // Whatever didn't fit
}

bool UStableInventory::HasItem_Implementation(const UItem *Item)
{
	int32 Count = Item->GetCount();
	for (UItem *InSlot : Slots)
	{
		if (!!InSlot && InSlot->MatchesType(Item))
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

UItem *UStableInventory::TakeItem_Implementation(UItem *Item)
{
	UItem *Out = Item->Clone();
	Out->SetCount(0);

	// Walk backwards and take from the end first
	for (int i = Slots.Num() - 1; i >= 0 && Out->GetCount() < Item->GetCount(); i--)
	{
		UItem *InSlot = Slots[i];
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
				HeldItems[i] = nullptr;
				Out->Merge(InSlot);
			}
		}
	}

	return Out->IsEmpty() ? nullptr : Out;
}

UItem *UStableInventory::GetItemSlot_Implementation(int32 SlotIdx)
{
	UItem *Item = nullptr;
	if (SlotIdx < this->Slots.Num())
	{
		Item = Slots[SlotIdx];
	}
	return Item;
}

UItem *UStableInventory::TakeItemSlot_Implementation(int32 SlotIdx, int32 Count)
{
	UItem *Item = nullptr;
	if (SlotIdx < this->Slots.Num() && !!Slots[SlotIdx] && Count)
	{
		Item = this->Slots[SlotIdx]->Split(Count);

		if (this->Slots[SlotIdx]->IsEmpty())
		{
			Slots[SlotIdx] = nullptr;
		}
	}
	return Item;
}

int32 UStableInventory::AddItemCountSlot_Implementation(int32 SlotIdx, int32 Count)
{
	int32 Leftover = Count;
	if (SlotIdx < this->Slots.Num() && Slots[SlotIdx]) // No effect if no item is in slot...
	{
		Leftover = Slots[SlotIdx]->AddCount(Count);
	}
	return Leftover;
}

UItem *UStableInventory::AddItemSlot_Implementation(int32 SlotIdx, UItem *OtherItem)
{
	UItem *Leftover = OtherItem;
	if (SlotIdx < this->Slots.Num())
	{
		if (Slots[SlotIdx])
		{
			Leftover = Slots[SlotIdx]->Merge(OtherItem);
		}
		else
		{
			Slots[SlotIdx] = OtherItem->Clone();
			Leftover = nullptr;
		}
	}
	return Leftover;
}