#pragma once

// Basic storage bin! Can hold lots of a single type of item.

#include "CoreMinimal.h"

#include "LogicalPlatformComponent.h"
#include "Factory/Logical/Inventory/Item.h"
#include "Factory/Logical/Inventory/ItemHandler.h"
#include "Factory/Logical/Inventory/InventoryBase.h"

#include "LogicalBin.generated.h"

UCLASS(Blueprintable, Abstract)
class ULogicalBin : public ULogicalPlatformComponent, public IItemHandler, public IInventoryBase
{
	GENERATED_BODY()
private:

protected:

	// Item type in the bin. Can be null.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	UItemType *ItemType;

	// Current number of items in the bin.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	int32 ItemCount;

	// Max number of stacks this bin can hold.
	// Note this number can't be directly compared to ItemCount as its STACKS, not items.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxStackCount = -1;

	// Which mesh to use for the bin
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh *BinMesh;

	TSubclassOf<class UFactoryInventoryScreen> InventoryScreenClass;

	ULogicalBin();

	UItem *DisplaySingleItemCache;
	TArray<UItem *>DisplayMultiItemCache;

	virtual void RefreshDisplayItem();

	virtual bool GetClickOptions(APlayerCharacter *Player, float Distance, FVector ActorLocation, ClickOption **DefaultOptOut, TArray<ClickOption> *OptionsOut) override;

public:

	virtual ~ULogicalBin() = default;

	virtual EDataValidationResult IsDataValid(TArray<FText> & ValidationErrors) override;

	virtual void RemoveFromPlatform(ULogicalPlatform *Platform) override;

	virtual FDirectionFlagMap GetDefaultIncomingConnectionPorts() override { return FDirectionFlagMap(true); } // all true
	virtual FDirectionFlagMap GetDefaultOutgoingConnectionPorts() override { return FDirectionFlagMap(false); } // all false (no auto output)

	virtual APlatformComponent *SpawnWorldComponentInternal(UPlatform *Platform) override;

	UFUNCTION()
	UStaticMesh *GetBinMesh() { return BinMesh; }

	UFUNCTION()
	UItemType *GetItemType() { return ItemType; }

	UFUNCTION()
	int32 GetItemCount() { if (!ItemType) return 0;  return ItemCount; }

	UFUNCTION()
	int32 GetMaxStackCount() { if (!ItemType) return 0;  return MaxStackCount; }

	UFUNCTION()
	int32 GetMaxItemCount() { if (!ItemType) return 0; return ItemType->GetMaxStackSize() * MaxStackCount; }

	UFUNCTION()
	float GetFullness() { if (!ItemType) return 0; return (float)GetItemCount() / (float)GetMaxItemCount(); }

	virtual bool ShouldHighlight(APlayerCharacter *Player, float Distance) override;

	// IItemHandler
public:

	virtual bool CanAccept_Implementation(EDirection Direction, const UItem *ItemIn) override;

	virtual UItem *InsertItem_Implementation(EDirection Direction, UItem *ItemIn) override;

	virtual void PeekItems_Implementation(TArray<UItem*> &ItemArray) override;

	virtual bool CanTake_Implementation(EDirection Direction, const UItem *ItemDemandOpt) override;

	virtual UItem *TakeItem_Implementation(EDirection Direction, const UItem *ItemDemandOpt) override;

	// IInventoryBase
public:
	virtual TArray<UItem*> GetItems_Implementation () override;
	virtual bool CanFit_Implementation (const UItem *Item) override;
	virtual UItem *AddItem_Implementation (UItem *ItemIn) override;
	virtual bool HasItem_Implementation (const UItem *Item) override;
	virtual UItem *TakeItem_Implementation (UItem *Item) override;
	virtual int32 GetMaxSlots_Implementation () override;
	virtual UItem *TakeItemSlot_Implementation (int32 SlotIdx, int32 Count) override;
	virtual int32 AddItemCountSlot_Implementation (int32 SlotIdx, int32 Count) override;
	virtual UItem *AddItemSlot_Implementation (int32 SlotIdx, UItem *ItemIn) override;
	virtual UItem *GetItemSlot_Implementation (int32 SlotIdx) override;

};