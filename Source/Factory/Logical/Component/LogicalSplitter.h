#pragma once

// Splitter

#include "CoreMinimal.h"

//#include "Factory/Logical/Inventory/Item.h"
#include "LogicalPlatformComponent.h"
#include "Factory/Logical/Inventory/ItemHandler.h"

#include "LogicalSplitter.generated.h"

UCLASS(Blueprintable, Abstract)
class ULogicalSplitter : public ULogicalPlatformComponent, public IItemHandler
{
	GENERATED_BODY()
private:
	void PushToReceivers();

protected:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	UItem *Item;

	int32 LastIncomeTick = 0;

	// Next output direction. Used for round robin behavior.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	EDirection NextOutputDirection;

	IItemHandler *CachedForwardReceiver;
	IItemHandler *CachedRightReceiver;
	IItemHandler *CachedLeftReceiver;
	IItemHandler *CachedRearProducer;

	ULogicalSplitter();

	void SetItem(UItem *NewItem, EDirection FromDirection = EDirection::MAX);
	void ClearItem();

public:

	// Change the belt direction. This forces a refresh of nearby tiles.
	UFUNCTION(BlueprintCallable)
	void SetDirection(EDirection NewDirection) { Direction = NewDirection; RefreshConnections(); }

	virtual void RegisterPlatform(ULogicalPlatform *Platform, FGridPosition GridPosition) override;

	virtual void RemoveFromPlatform(ULogicalPlatform *Platform) override;

	virtual bool IsActiveDuring(EGamePhase Phase) override;

	virtual void ShopTick(EGamePhase Phase) override;

	virtual bool RefreshNearby(FLocalLayout NearbyLayout) override;

	virtual FDirectionFlagMap GetDefaultIncomingConnectionPorts() override { return FDirectionFlagMap(false, false, true, false); } // only west
	virtual FDirectionFlagMap GetDefaultOutgoingConnectionPorts() override { return FDirectionFlagMap(true, true, false, true); } // all but west

	// Return what belt is pointing towards.
	IItemHandler *GetForwardReceivingHandler() { return CachedForwardReceiver; }
	IItemHandler *GetRightReceivingHandler() { return CachedRightReceiver; }
	IItemHandler *GetLeftReceivingHandler() { return CachedLeftReceiver; }
	IItemHandler *GetRearProducerHandler() { return CachedRearProducer; }

	virtual UItem *GetItem() { return Item; }
	virtual EDirection GetNextOutputDirection() { return NextOutputDirection; }

	// IItemHandler
public:

	virtual APlatformComponent *SpawnWorldComponentInternal(UPlatform *Platform) override;

	virtual bool CanAccept_Implementation(EDirection Direction, const UItem *ItemIn) override;

	virtual UItem *InsertItem_Implementation(EDirection Direction, UItem *ItemIn) override;

	virtual void PeekItems_Implementation(TArray<UItem*> &ItemArray) override;

	virtual bool CanTake_Implementation(EDirection Direction, const UItem *ItemDemandOpt) override;

	virtual UItem *TakeItem_Implementation(EDirection Direction, const UItem *ItemDemandOpt) override;

};