#pragma once

// Conveyor belt!

#include "CoreMinimal.h"

//#include "Factory/Logical/Inventory/Item.h"
#include "LogicalPlatformComponent.h"
#include "Factory/Logical/Inventory/ItemHandler.h"

#include "LogicalBelt.generated.h"

UCLASS(Blueprintable, Abstract)
class ULogicalBelt : public ULogicalPlatformComponent, public IItemHandler
{
	GENERATED_BODY()
private:

protected:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	UItem *Item;

	// Position of current item on the belt, from 0 to 1.
	// 1 indicates the item will be pushed onto next item if one is present.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	float fItemProgress;

	// How many ticks it takes for an item to move across this belt.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float fSpeedTicks;

	int32 LastIncomeTick = 0;

	EDirection LastIncomeDirection;

	float GetProgressPerTick() { return 1.0f / fSpeedTicks; }

	IItemHandler *CachedReceiver;
	IItemHandler *CachedRearProducer;
	bool bCachedExtended; // If we're connected to the side of another belt

	ULogicalBelt();

	void SetItem(UItem *NewItem, EDirection FromDirection = EDirection::MAX);
	void ClearItem();

public:

	//static ULogicalBelt *MakeBelt(EDirection Direction);
	
	// Change the belt direction. This forces a refresh of nearby tiles.
	UFUNCTION(BlueprintCallable)
	void SetDirection(EDirection NewDirection) { Direction = NewDirection; RefreshConnections(); }

	virtual void RegisterPlatform(ULogicalPlatform *Platform, FGridPosition GridPosition) override;

	virtual void RemoveFromPlatform(ULogicalPlatform *Platform) override;

	virtual bool IsActiveDuring(EGamePhase Phase) override;

	virtual void ShopTick(EGamePhase Phase) override;

	virtual bool RefreshNearby(FLocalLayout NearbyLayout) override;

	virtual FDirectionFlagMap GetDefaultIncomingConnectionPorts() override { return FDirectionFlagMap(false, true, true, true); } // all but east
	virtual FDirectionFlagMap GetDefaultOutgoingConnectionPorts() override { return FDirectionFlagMap(true, false, false, false); } // only east

	// Return what belt is pointing towards.
	IItemHandler *GetReceivingHandler() { return CachedReceiver; }
	IItemHandler *GetRearProducerHandler() { return CachedRearProducer; }
	bool GetIsExtended() { return bCachedExtended; }

	virtual UItem *GetItem() { return Item; }
	virtual float GetItemProgress(float PartialTicks = 0.0f);
	virtual EDirection GetLastInputDirection() { return LastIncomeDirection; }

	// IItemHandler
public:

	virtual APlatformComponent *SpawnWorldComponentInternal(UPlatform *Platform) override;

	virtual bool CanAccept_Implementation(EDirection Direction, const UItem *ItemIn) override;

	virtual UItem *InsertItem_Implementation(EDirection Direction, UItem *ItemIn) override;

	virtual void PeekItems_Implementation(TArray<UItem*> &ItemArray) override;

	virtual bool CanTake_Implementation(EDirection Direction, const UItem *ItemDemandOpt) override;

	virtual UItem *TakeItem_Implementation(EDirection Direction, const UItem *ItemDemandOpt) override;

};