#pragma once

// Basic storage bin! Can hold lots of a single type of item.

#include "CoreMinimal.h"

#include "LogicalPlatformComponent.h"
#include "Factory/Logical/Item.h"
#include "Factory/Logical/ItemHandler.h"

#include "LogicalBin.generated.h"

UCLASS(Blueprintable, Abstract)
class ULogicalBin : public ULogicalPlatformComponent, public IItemHandler
{
	GENERATED_BODY()
private:

protected:

	// Item type in the bin. Can be null.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UItemType *ItemType;

	// Current number of items in the bin.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ItemCount;

	// Max number of stacks this bin can hold.
	// Note this number can't be directly compared to ItemCount as its STACKS, not items.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxStackCount = -1;

	// Which mesh to use for the bin
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh *BinMesh;

	ULogicalBin() : ULogicalPlatformComponent() {};

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

	// IItemHandler
public:

	virtual bool CanAccept_Implementation(EDirection Direction, const UItem *ItemIn) override;

	virtual UItem *InsertItem_Implementation(EDirection Direction, UItem *ItemIn) override;

	virtual void PeekItems_Implementation(TArray<UItem*> &ItemArray) override;

	virtual bool CanTake_Implementation(EDirection Direction, const UItem *ItemDemandOpt) override;

	virtual UItem *TakeItem_Implementation(EDirection Direction, const UItem *ItemDemandOpt) override;

};