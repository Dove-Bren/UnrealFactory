#include "Item.h"

UItem::UItem()
{

}

/*static*/ UItem *UItem::MakeItem(UItemType *Type)
{
	UItem *Item = NewObject<UItem>();
	Item->SetType(Type);
	Item->SetCount(1); // Default to 1
	return Item;
}

int32 UItem::SetCount(int32 NewCount)
{
	Count = (NewCount < 0 ? 0 : (NewCount > GetMaxCount() ? GetMaxCount() : NewCount));
	return Count;
}

int32 UItem::AddCount(int32 AddCount)
{
	// Positive naturally means add more. Negative means take away.
	// If too many are added, we want to return how many didn't fit.
	// If negative has higher magnitude than count, we want to return
	// (negative) how many couldn't be taken.

	// How much it would be with no restriction
	int32 Raw = AddCount + Count;
	Count = (Raw < 0 ? 0 : (Raw > GetMaxCount() ? GetMaxCount() : Raw)); // Clamped to [0 - GetMaxCount()]

	// Return difference
	// For example, -2 Raw + 0 Count = 2 that couldn't be taken away = -2
	// 5 Raw + 2 count = 3 that coulnd't be added = 3
	// Raw - Count
	return Raw - Count;
}

UItem *UItem::Split(int32 SplitCount)
{
	UItem *NewItem = nullptr;

	if (SplitCount < 1)
	{
		SplitCount = 1;
	}

	if (!IsEmpty())
	{
		int32 Avail = (Count > SplitCount ? SplitCount : Count);
		NewItem = MakeItem(ItemType);
		NewItem->SetCount(Avail);
		this->SetCount(Count - Avail); // May be 0
	}

	return NewItem;
}

UItem *UItem::Merge(const UItem *OtherItem)
{
	UItem *NewItem = nullptr;

	if (OtherItem && MatchesType(OtherItem) && !OtherItem->IsEmpty())
	{
		int32 Leftover = this->AddCount(OtherItem->GetCount());
		if (Leftover)
		{
			NewItem = MakeItem(ItemType);
			NewItem->SetCount(Leftover);
		}
	}

	return NewItem;
}