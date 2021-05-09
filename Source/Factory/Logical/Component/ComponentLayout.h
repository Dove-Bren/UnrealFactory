#pragma once

// Wrapper for component placement and lookup.
// Doesn't have any solid bounds and lets you just add wherever you want. User should handle bounds.

#include "CoreMinimal.h"

#include "Factory/Layout2D.h"
#include "LogicalPlatformComponent.h"

#include "ComponentLayout.generated.h"

UCLASS()
class UComponentLayout : public UObject, public Layout2D<ULogicalPlatformComponent>
{
	GENERATED_BODY()
private:



protected:

	// Quick access list of all components that are part of this layout
	UPROPERTY(VisibleAnywhere)
	TArray<ULogicalPlatformComponent*> Components;

public:

	UComponentLayout() : UObject(), Layout2D() {};

	virtual ULogicalPlatformComponent *Remove(int X, int Y) override;

	virtual ULogicalPlatformComponent *Insert(int X, int Y, ULogicalPlatformComponent *Element, bool bReplace) override;

	// Get the component in the provided cell.
	UFUNCTION(BlueprintCallable)
	ULogicalPlatformComponent *GetBP(int CellX, int CellY);

	// Removes a component from the grid, returning it if there was one in that cell.
	UFUNCTION(BlueprintCallable)
	ULogicalPlatformComponent *RemoveBP(int CellX, int CellY);

	// Insert a component onto the grid. If Replace is true, will remove any existing
	// component in that cell and return it.
	UFUNCTION(BlueprintCallable)
	ULogicalPlatformComponent *InsertBP(int CellX, int CellY, ULogicalPlatformComponent *Element, bool bReplace);

	// Return the list of components on this grid
	UFUNCTION(BlueprintCallable)
	TArray<ULogicalPlatformComponent*> GetComponents();

};