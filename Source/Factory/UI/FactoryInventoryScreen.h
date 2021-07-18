#pragma once

// Factory overlay screen that takes an extra inventory as a parameter

#include "CoreMinimal.h"

#include "FactoryHUDWidget.h"
#include "Factory/Logical/Inventory/InventoryBase.h"

#include "FactoryInventoryScreen.generated.h"

UCLASS(Abstract)
class UFactoryInventoryScreen : public UFactoryHUDWidget
{
	GENERATED_BODY()

public:

	// Set the inventory this screen will display
	UFUNCTION(BlueprintCallable)
	void SetInventory(TScriptInterface<IInventoryBase> InventoryIn) { Inventory = InventoryIn; }

	// Get the current inventory ref
	UFUNCTION(BlueprintCallable)
	TScriptInterface<IInventoryBase> GetInventory() { return Inventory; }

protected:

	// The inventory (not the character's inventory) this screen should display
	UPROPERTY(VisibleAnywhere)
	TScriptInterface<IInventoryBase> Inventory;

};