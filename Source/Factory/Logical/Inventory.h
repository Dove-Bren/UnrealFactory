#pragma once

#include "CoreMinimal.h"

#include "Engine/GameInstance.h"

#include "Factory/Logical/ItemType.h"
#include "Factory/Logical/Item.h"

#include "Inventory.generated.h"

typedef class UInventorySlotRef;

UCLASS(BlueprintType, Blueprintable)
class UInventory : public UObject
{
	GENERATED_BODY()

	friend class UInventorySlotRef;

public:

	UInventory();
	~UInventory();

	UFUNCTION(BlueprintCallable)
	virtual const TArray<UItem*> GetItems() const { return HeldItems; }

	// Check if the provided item would be able to fit completely in the given inventory
	// given stack and slot constraints.
	UFUNCTION(BlueprintCallable)
	virtual bool CanFit(const UItem *Item);

	// Attept to add an item to an inventory.
	// Works with inventory slot constraints (and individual item stack constraints)
	// which may mean not all of the item fits.
	// Returns any leftover item.
	UFUNCTION(BlueprintCallable)
	virtual UItem *AddItem(UItem *ItemIn);

	// Check whether [Item->Type] items exist of quantity [Item->Count] or more.
	UFUNCTION(BlueprintCallable)
	virtual bool HasItem(const UItem *Item);

	// Attempt to remove some of an item from the inventory.
	// Removes up to [Item->Count] items of type [Item->Type] from the inventory.
	// If not enough exists, only removes what it has.
	// Returns the items removed.
	UFUNCTION(BlueprintCallable)
	virtual UItem *TakeItem(UItem *Item);

	// Return the maximum number of slots this inventory can hold.
	UFUNCTION(BlueprintCallable)
	virtual int32 GetMaxSlots() { return 20; }

	/// Harder to use slot interface ///

	// Attempt to remove some count of an item from the specified slot.
	// Returns all of what was actually removed, including a nullptr or
	// an item with a count less than Count if there wasn't anything/enough
	// in the slot.
	UFUNCTION(BlueprintCallable)
	virtual UItem *TakeItemSlot(int32 SlotIdx, int32 Count);

	// Attempt to add some count to the item in the specified slot.
	// This obeys stack limit restrictions and will not add more than is allowed.
	// It also cannot add to a slot that has no item.
	// Returns how many of Count could NOT be added.
	UFUNCTION(BlueprintCallable)
	virtual int32 AddItemCountSlot(int32 SlotIdx, int32 Count);

	// Attempt to add an item into the given slot.
	// This obeys stack limit restrictions and will not add more than is allowed.
	// Similarly, items of a different type than what's in the slot cannot be added.
	// NOTE: Some inventories (default implementation) use sparse lists and do not support slots
	// with an index that's non-contiguous, meaning this method may not add to the slot provided
	// and instead add to a smaller one.
	// This function returns whatever could NOT be added.
	UFUNCTION(BlueprintCallable)
	virtual UItem *AddItemSlot(int32 SlotIdx, UItem *ItemIn);

	// Return the item in the given slot.
	UFUNCTION(BlueprintCallable)
	virtual UItem *GetItemSlot(int32 SlotIdx);

protected:

	// General set of items held in the inventory in no particular slot
	UPROPERTY(VisibleAnywhere)
	TArray<UItem*> HeldItems;

private:


};

// Convenient struct for referring to a specific slot in an inventory
UCLASS(BlueprintType)
class UInventorySlotRef : public UObject
{
	GENERATED_BODY()

protected:

	UInventorySlotRef() {};
	virtual ~UInventorySlotRef() = default;

	UInventorySlotRef *Setup(UInventory *InventoryIn, int32 SlotIdxIn) { Inventory = InventoryIn; SlotIdx = SlotIdxIn; return this; }

	// Inventory we're pointing at
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventory *Inventory;

	// Slot index we're referring to
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 SlotIdx = -1;

public:

	UFUNCTION(BlueprintCallable)
	static UInventorySlotRef *MakeRef(UObject *Outer, UInventory *InventoryIn, int32 SlotIdxIn) { UInventorySlotRef *Ref = NewObject<UInventorySlotRef>(Outer ? Outer : GetTransientPackage()); Ref->Setup(InventoryIn, SlotIdxIn); return Ref; }

	// Check if we still should refer to a valid item
	UFUNCTION(BlueprintCallable)
	bool IsValid() const { return Inventory && SlotIdx >= 0; }

	// Return the item in the slot we're pointing to
	UFUNCTION(BlueprintCallable)
	UItem *GetItem() const { if (IsValid()) return Inventory->GetItemSlot(SlotIdx); else return nullptr; }

	// Remove count from the item slot we're pointed at, possibly exhausting it.
	// Return a new item with the removed count.
	UFUNCTION(BlueprintCallable)
	UItem *RemoveItems(int32 Count);

	// Attempt to add some count to the item slot we're pointed at.
	// This obeys stack limit restrictions and returns how many we couldn't
	// add.
	UFUNCTION(BlueprintCallable)
	int32 AddItemCount(int32 Count) { if (IsValid()) return Inventory->AddItemCountSlot(SlotIdx, Count); return Count; }

	// Attempt to merge in another item stack into the one we're pointed at.
	// This obeys stack limit restrictions, and can't merge items of different types.
	// Returns anything that couldn't be added.
	UFUNCTION(BlueprintCallable)
	UItem *AddItem(UItem *OtherItem) { if (IsValid()) return Inventory->AddItemSlot(SlotIdx, OtherItem); return nullptr; }

	// Another name for AddItem to match Item interface
	UFUNCTION(BlueprintCallable)
	UItem *MergeItem(UItem *OtherItem) { return AddItem(OtherItem); }

};