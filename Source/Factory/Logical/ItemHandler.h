#pragma once

// An item object, like an "ItemStack" in Minecraft.
// Has an item type and a count.
// This logical piece has no mesh, etc.

#include "CoreMinimal.h"

#include "Item.h"

#include "Factory/GameEnums.h"

#include "ItemHandler.generated.h"

UINTERFACE(Blueprintable)
class UItemHandler : public UInterface
{
	GENERATED_BODY()
};

class IItemHandler
{
	GENERATED_BODY()

public:

	// Check whether this handler can accept the provided item from the specified direction.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanAccept(EDirection Direction, const UItem *ItemIn);

	// Insert the provided item from the given direction.
	// If not all of the item will fit, returns the leftover amount.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UItem *InsertItem(EDirection Direction, UItem *ItemIn);

	// Get a snapshot of all items currently 'inside' this handler.
	// Array is filled with any items. Item refs are copies and can be modified freely.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PeekItems(TArray<UItem*> &ItemArray);

	// Check if this handler can provide items in the given direction.
	// If the optional 'ItemDemandOpt' is provided, checks if specifically that item (and quantity)
	// can be drawn. Otherwise, returns if any items of any quantity can be drawn.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanTake(EDirection Direction, const UItem *ItemDemandOpt);

	// Attempt to take an item from the handler from the given direction.
	// Like CanTake, can optionally specify which item (and how many) to take.
	// Returns the items drawn, including an item with a lower quantity than requested if
	// not enough of the demanded item existed.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UItem *TakeItem(EDirection Direction, const UItem *ItemDemandOpt);

	// Wrap up calling CanAccept and then InsertItem on a handler.
	// Handler must cast cleanly to a UObject.
	static UItem *AttemptInsert(IItemHandler *Handler, EDirection Direction, UItem *InsertItem);
};

/*static*/ inline UItem *IItemHandler::AttemptInsert(IItemHandler *Handler, EDirection Direction, UItem *InsertItem)
{
	UItem *Leftover = InsertItem;
	UObject *Comp = Cast<UObject>(Handler);
	check(Comp);
	if (Comp && Handler->Execute_CanAccept(Comp, Direction, InsertItem))
	{
		Leftover = Handler->Execute_InsertItem(Comp, Direction, InsertItem);
	}

	return Leftover;
}