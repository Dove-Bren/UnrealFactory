#pragma once

// Wrapper for component placement and lookup.
// Doesn't have any solid bounds and lets you just add wherever you want. User should handle bounds.

#include "CoreMinimal.h"

#include "Factory/Layout2D.h"
#include "PlatformComponent.h"

#include "ComponentLayout.generated.h"

UCLASS()
class UComponentLayout : public UObject, public Layout2D<APlatformComponent>
{
	GENERATED_BODY()
private:



protected:

	// Quick access list of all components that are part of this layout
	UPROPERTY(VisibleAnywhere)
	TArray<APlatformComponent*> Components;

public:

	UComponentLayout() : UObject(), Layout2D() {};

	virtual APlatformComponent *Remove(int X, int Y) override;

	virtual APlatformComponent *Insert(int X, int Y, APlatformComponent *Element, bool bReplace) override;

	// Get the component in the provided cell.
	UFUNCTION(BlueprintCallable)
	APlatformComponent *GetBP(int CellX, int CellY);

	// Removes a component from the grid, returning it if there was one in that cell.
	UFUNCTION(BlueprintCallable)
	APlatformComponent *RemoveBP(int CellX, int CellY);

	// Insert a component onto the grid. If Replace is true, will remove any existing
	// component in that cell and return it.
	UFUNCTION(BlueprintCallable)
	APlatformComponent *InsertBP(int CellX, int CellY, APlatformComponent *Element, bool bReplace);

	// Return the list of components on this grid
	UFUNCTION(BlueprintCallable)
	TArray<APlatformComponent*> GetComponents();

};