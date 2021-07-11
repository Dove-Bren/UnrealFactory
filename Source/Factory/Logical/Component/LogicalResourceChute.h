#pragma once

// Chute for taking items out of a shop's global storage on the factory floor!

#include "CoreMinimal.h"

#include "LogicalPlatformComponent.h"
#include "Factory/GameEnums.h"
#include "Factory/Logical/Item.h"
#include "Factory/Logical/ItemHandler.h"
#include "Factory/Logical/ItemType.h"
#include "Factory/Logical/LogicalShop.h"
#include "Factory/Logical/Component/LogicalBin.h"

#include "LogicalResourceChute.generated.h"

UCLASS(Blueprintable, Abstract)
class ULogicalResourceChute :public ULogicalPlatformComponent, public IItemHandler
{
	GENERATED_BODY()
private:

protected:

	// The type of resource this chute can dispense
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EResourceType ResourceType;

	// The actual item type to dispense for each count of ResourceType withdrawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UItemType> ItemType;

	// Which mesh to use for the bin
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh *ChuteMesh;

	ULogicalResourceChute() : ULogicalPlatformComponent() {};

	ULogicalShop *GetShop();

public:

	virtual ~ULogicalResourceChute() = default;

	virtual EDataValidationResult IsDataValid(TArray<FText> & ValidationErrors) override;

	virtual bool IsActiveDuring(EGamePhase Phase) override { return true; }
	virtual void ShopTick(EGamePhase Phase) override;

	virtual FDirectionFlagMap GetDefaultIncomingConnectionPorts() override { return FDirectionFlagMap(false); } // all false (no input allowed)
	virtual FDirectionFlagMap GetDefaultOutgoingConnectionPorts() override { return FDirectionFlagMap(true); } // all true (auto push to nearby)

	virtual APlatformComponent *SpawnWorldComponentInternal(UPlatform *Platform) override;

	UFUNCTION()
	UStaticMesh *GetChuteMesh() { return ChuteMesh; }

	UFUNCTION()
	TSubclassOf<UItemType> GetItemType() { return ItemType; }

	UFUNCTION()
	EResourceType GetResourceType() { return ResourceType; }

	// IItemHandler
public:

	virtual bool CanAccept_Implementation(EDirection Direction, const UItem *ItemIn) override;

	virtual UItem *InsertItem_Implementation(EDirection Direction, UItem *ItemIn) override;

	void PeekItems_Implementation(TArray<UItem*> &ItemArray) override;

	virtual bool CanTake_Implementation(EDirection Direction, const UItem *ItemDemandOpt) override;

	virtual UItem *TakeItem_Implementation(EDirection Direction, const UItem *ItemDemandOpt) override;

};