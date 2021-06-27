#pragma once

// An item object, like an "ItemStack" in Minecraft.
// Has an item type and a count.
// This logical piece has no mesh, etc.

#include "CoreMinimal.h"

#include "ItemType.h"

#include "Factory/GameEnums.h"

#include "Item.generated.h"

UCLASS(Blueprintable)
class UItem : public UObject
{
	GENERATED_BODY()
private:

	//// Link to physical item actor
	//UPROPERTY(VisibleAnywhere)
	//AItem *LinkedActor;

	// Link to physical item actor
	UPROPERTY(VisibleAnywhere)
	UItemType *ItemType;

	// Link to physical item actor
	UPROPERTY(VisibleAnywhere)
	int32 Count;

protected:

	UItem();

	void SetType(UItemType *Type) { this->ItemType = Type; }

public:

	UFUNCTION(BlueprintCallable)
	static UItem *MakeItemEx(UObject *Outer, UItemType *Type);

	UFUNCTION(BlueprintCallable)
	static UItem *MakeItem(UObject *Outer, TSubclassOf<UItemType> Type);

	UFUNCTION(BlueprintCallable)
	UItemType *GetType() const { return this->ItemType; }

	UFUNCTION(BlueprintCallable)
	int32 GetCount() const { return this->Count; }

	UFUNCTION(BlueprintCallable)
	int32 GetMaxCount() const { return this->ItemType ? this->ItemType->GetMaxStackSize() : 0; };

	// Check if the item no longer holds any count and doesn't represent any actual number of items.
	UFUNCTION(BlueprintCallable)
	bool IsEmpty() const { return this->Count <= 0; }

	// Sets the count on an item.
	// This respects max stack size.
	// Returns the actual count set to the item after stack size limit clamping.
	int32 SetCount(int32 NewCount);

	// Adds some count to this item instance, respecting max stack size.
	// AddCount can be negative to take count away.
	// Returns the amount that wasn't able to be added.
	UFUNCTION(BlueprintCallable)
	int32 AddCount(int32 AddCount);

	// Take away the number of items and put them into their own Item instance that's a copy of this one,
	// returning the new one.
	// This modifies the original item! Original item may end up being empty after this call.
	// This never returns a pointer to the original item.
	UFUNCTION(BlueprintCallable)
	UItem *Split(int32 SplitCount = 1);

	// Check whether the provided item could merge _with no leftovers_ into
	// this item stack.
	// False indicates either the types don't match or adding the extra count
	// would overflow the max stack size.
	// In other words, a call to Merge would produce leftovers.
	UFUNCTION(BlueprintCallable)
	bool CanMerge(const UItem *OtherItem);

	// Attempt to add another Item into this one respecting stack sizes.
	// If item types do not match, this will do nothing.
	// Anything left over is returned. If nothing is left, nullptr is returned.
	// Note passed in item is not changed nor will it be returned.
	UFUNCTION(BlueprintCallable)
	UItem *Merge(const UItem *OtherItem);

	// Create a duplicate item with the same count
	UFUNCTION(BlueprintCallable)
	UItem *Clone() const { if (IsEmpty()) return nullptr; UItem *Item = MakeItemEx(this->GetOuter(), ItemType); Item->SetCount(Count); return Item; }

	// Check if another item has the same item type as this one.
	UFUNCTION(BlueprintCallable)
	bool MatchesType(const UItem *OtherItem) const { return OtherItem && OtherItem->ItemType == this->ItemType; }

	// Check if this item is the same type and holds at least as much count as OtherItem.
	UFUNCTION(BlueprintCallable)
	bool StackContains(const UItem *OtherItem) const { return MatchesType(OtherItem) && OtherItem->GetCount() < this->GetCount(); }

};