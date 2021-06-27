#pragma once

#include "CoreMinimal.h"

#include "Engine/GameInstance.h"

#include "Factory/Logical/ItemType.h"
#include "Factory/Logical/Item.h"

#include "Inventory.generated.h"

UCLASS(BlueprintType, Blueprintable)
class UInventory : public UObject
{
	GENERATED_BODY()

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

protected:

	// General set of items held in the inventory in no particular slot
	UPROPERTY(VisibleAnywhere)
	TArray<UItem*> HeldItems;

private:


};