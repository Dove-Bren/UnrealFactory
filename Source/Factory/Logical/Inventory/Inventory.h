#pragma once

#include "CoreMinimal.h"

//#include "Engine/GameInstance.h"

//#include "Factory/Logical/Inventory/ItemType.h"
//#include "Factory/Logical/Inventory/Item.h"
//#include "Factory/Logical/Inventory/ItemHandler.h"

#include "InventoryBase.h"

#include "Inventory.generated.h"

typedef class UInventorySlotRef;
typedef class IItemHandler;
typedef class UItem;
typedef class UItemType;

UCLASS(BlueprintType, Blueprintable)
class UInventory : public UObject, public IInventoryBase
{
	GENERATED_BODY()

public:

	UInventory();
	virtual ~UInventory();

	virtual TArray<UItem*> GetItems_Implementation() override { return HeldItems; }

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

	// General set of items held in the inventory in no particular slot
	UPROPERTY(VisibleAnywhere)
	TArray<UItem*> HeldItems;

private:


};