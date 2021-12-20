#pragma once

#include "CoreMinimal.h"

#include "Inventory.h"

#include "StableInventory.generated.h"

typedef class UInventorySlotRef;
typedef class IItemHandler;
typedef class UItem;
typedef class UItemType;

// A stable (non-sparse) inventory implementation.
// Has actual slots, and allows gaps between filled slots and non-filled slots.
// Better user experience than sparse lists.
UCLASS(BlueprintType, Blueprintable)
class UStableInventory : public UInventory
{
	GENERATED_BODY()

public:

	UStableInventory();
	virtual ~UStableInventory();

	virtual TArray<UItem*> GetItems_Implementation() override { return Slots; }

	virtual bool CanFit_Implementation(const UItem *Item) override;

	virtual UItem *AddItem_Implementation(UItem *ItemIn) override;

	virtual bool HasItem_Implementation(const UItem *Item) override;

	virtual UItem *TakeItem_Implementation(UItem *Item) override;

	virtual int32 GetMaxSlots_Implementation() override { return 20; }

	virtual UItem *TakeItemSlot_Implementation(int32 SlotIdx, int32 Count) override;

	virtual int32 AddItemCountSlot_Implementation(int32 SlotIdx, int32 Count) override;

	virtual UItem *AddItemSlot_Implementation(int32 SlotIdx, UItem *ItemIn) override;

	virtual UItem *GetItemSlot_Implementation(int32 SlotIdx) override;

protected:

	// Inventory slots. Slots may be empty (nullptr/invalid item inside).
	UPROPERTY(VisibleAnywhere)
	TArray<UItem*> Slots;

private:


};