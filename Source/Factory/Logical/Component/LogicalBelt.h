#pragma once

// Conveyor belt!

#include "CoreMinimal.h"

//#include "Factory/Item/Item.h"
#include "LogicalPlatformComponent.h"
#include "Factory/Logical/ItemHandler.h"

#include "LogicalBelt.generated.h"

UCLASS()
class ULogicalBelt : public ULogicalPlatformComponent, public IItemHandler
{
	GENERATED_BODY()
private:

	// Direction the belt is facing (and pushing)
	UPROPERTY(VisibleAnywhere)
	EDirection Direction;

protected:

	UPROPERTY(VisibleAnywhere)
	UItem *Item;

	// Local cache of nearby item receiving machines, etc. -- possibly other belts?

	ULogicalBelt() : ULogicalPlatformComponent() {};

public:

	static ULogicalBelt *MakeBelt(EDirection Direction);
	
	// Change the belt direction. This forces a refresh of nearby tiles.
	UFUNCTION(BlueprintCallable)
	void SetDirection(EDirection NewDirection) { Direction = NewDirection; RefreshConnections(); }

	virtual void RegisterPlatform(ULogicalPlatform *Platform, FGridPosition GridPosition) override;

	virtual void RemoveFromPlatform(ULogicalPlatform *Platform) override;

	virtual bool IsActiveDuring(EGamePhase Phase) override;

	virtual void ShopTick(EGamePhase Phase) override;

	virtual void RefreshNearby(FLocalLayout NearbyLayout) override;

	IItemHandler *GetReceivingHandler(); // Return what belt is pointing towards.

	// IItemHandler
public:

	virtual bool CanAccept_Implementation(EDirection Direction, const UItem *ItemIn) override;

	virtual UItem *InsertItem_Implementation(EDirection Direction, UItem *ItemIn) override;

	void PeekItems_Implementation(TArray<UItem*> &ItemArray) override;

	virtual bool CanTake_Implementation(EDirection Direction, const UItem *ItemDemandOpt) override;

	virtual UItem *TakeItem_Implementation(EDirection Direction, const UItem *ItemDemandOpt) override;

};