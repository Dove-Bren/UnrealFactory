#pragma once

// Conveyor belt merger

#include "CoreMinimal.h"

//#include "Factory/Logical/Inventory/Item.h"
#include "LogicalPlatformComponent.h"
#include "Factory/Logical/Inventory/ItemHandler.h"

#include "LogicalMerger.generated.h"

UCLASS(Blueprintable, Abstract)
class ULogicalMerger : public ULogicalPlatformComponent, public IItemHandler
{
	GENERATED_BODY()
private:
	void PullFromProducers();

protected:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	UItem *Item;

	int32 LastIncomeTick = 0;

	// Next input direction. Used for round-robin behavior.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	EDirection NextIncomeDirection; 

	IItemHandler *CachedReceiver;
	IItemHandler *CachedRearProducer;
	IItemHandler *CachedLeftProducer;
	IItemHandler *CachedRightProducer;

	ULogicalMerger();

	void SetItem(UItem *NewItem, EDirection FromDirection = EDirection::MAX);
	void ClearItem();

public:

	// Change the merger input direction. This forces a refresh of nearby tiles.
	UFUNCTION(BlueprintCallable)
	void SetDirection(EDirection NewDirection) { Direction = NewDirection; RefreshConnections(); }

	virtual void RegisterPlatform(ULogicalPlatform *Platform, FGridPosition GridPosition) override;

	virtual void RemoveFromPlatform(ULogicalPlatform *Platform) override;

	virtual bool IsActiveDuring(EGamePhase Phase) override;

	virtual void ShopTick(EGamePhase Phase) override;

	virtual bool RefreshNearby(FLocalLayout NearbyLayout) override;

	virtual FDirectionFlagMap GetDefaultIncomingConnectionPorts() override { return FDirectionFlagMap(false, true, true, true); } // all but east
	virtual FDirectionFlagMap GetDefaultOutgoingConnectionPorts() override { return FDirectionFlagMap(true, false, false, false); } // only east

	// Return what merger is pointing towards.
	IItemHandler *GetReceivingHandler() { return CachedReceiver; }
	IItemHandler *GetRearProducerHandler() { return CachedRearProducer; }
	IItemHandler *GetLeftProducerHandler() { return CachedLeftProducer; }
	IItemHandler *GetRightProducerHandler() { return CachedRightProducer; }

	virtual UItem *GetItem() { return Item; }
	virtual EDirection GetNextInputDirection() { return NextIncomeDirection; }

	// IItemHandler
public:

	virtual APlatformComponent *SpawnWorldComponentInternal(UPlatform *Platform) override;

	virtual bool CanAccept_Implementation(EDirection Direction, const UItem *ItemIn) override;

	virtual UItem *InsertItem_Implementation(EDirection Direction, UItem *ItemIn) override;

	virtual void PeekItems_Implementation(TArray<UItem*> &ItemArray) override;

	virtual bool CanTake_Implementation(EDirection Direction, const UItem *ItemDemandOpt) override;

	virtual UItem *TakeItem_Implementation(EDirection Direction, const UItem *ItemDemandOpt) override;

};