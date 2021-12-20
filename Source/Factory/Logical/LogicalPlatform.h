#pragma once

// A platform is a single vertical plane in the game. Each is mapped to a type (see GamePlatform).
// Platforms are responsible for managing the collections of all objects in the platform.
// Note that, for the Mine, this includes all of the machines belonging to one shop.

#include "CoreMinimal.h"

#include "Factory/GameEnums.h"
#include "Factory/GridPosition.h"
#include "Component/LocalLayout.h"
#include "Component/ComponentLayout.h"
#include "Component/LogicalPlatformComponent.h"

#include "LogicalPlatform.generated.h"

#define CELL_SIZE 100
#define MIN_PLATFORM_WIDTH 10
#define MIN_PLATFORM_HEIGHT 10

typedef class ULogicalShop ULogicalShop;

UCLASS()
class ULogicalPlatform : public UObject
{
	GENERATED_BODY()
private:

	// Type of platform
	UPROPERTY(VisibleAnywhere)
	EGamePlatform PlatformType;

protected:

	// Link to parent shop object
	UPROPERTY(VisibleAnywhere)
	ULogicalShop *ShopParent;

	// Grid
	UPROPERTY(VisibleAnywhere)
	UComponentLayout *Grid;

	// If true, prevents floor resizing and dynamic walls
	bool bStaticFloor = false;

	// Current Width
	UPROPERTY(VisibleAnywhere)
	float FloorWidth;

	// Current Height
	UPROPERTY(VisibleAnywhere)
	float FloorHeight;

public:
	ULogicalPlatform();

	virtual ~ULogicalPlatform() = default;

	UFUNCTION(BlueprintCallable)
	void AttachToShop(EGamePlatform Type, ULogicalShop *Shop);

	// Add a component to this layer
	UFUNCTION(BlueprintCallable)
	void AddComponent(FGridPosition Position, ULogicalPlatformComponent *Component);

	// Add a component to this layer
	UFUNCTION(BlueprintCallable)
	void AddComponentAt(float WorldX, float WorldY, ULogicalPlatformComponent *Component);

	// Get a component on this layer from its position
	UFUNCTION(BlueprintCallable)
	FLocalLayout GetComponent(FGridPosition Position);

	// Get a component on this layer from its WORLD position
	UFUNCTION(BlueprintCallable)
	FLocalLayout GetComponentAt(float WorldX, float WorldY);

	// Get the grid position at the given world position
	UFUNCTION(BlueprintCallable)
	FGridPosition GetGridPosFromWorld(float WorldX, float WorldY);

	// Get the world position of the given grid position
	UFUNCTION(BlueprintCallable)
	FVector GetWorldPosFromGrid(FGridPosition GridPos, bool bCentered = false);

	// Remove a component from the platform by supplying the component directly.
	UFUNCTION(BlueprintCallable)
	void RemoveComponent(ULogicalPlatformComponent *Component);

	// Remove the component at the given spot from the platform
	UFUNCTION(BlueprintCallable)
	void RemoveComponentAtWorldPos(float WorldX, float WorldY);

	// Remove the component at the given spot from the platform
	UFUNCTION(BlueprintCallable)
	void RemoveComponentAt(FGridPosition GridPos);

	// Transition to the provided state, including stopping any activities from the previous one
	UFUNCTION(BlueprintCallable)
	virtual void StartPhase(EGamePhase Phase);

	// Perform regular shop tick functions for the platform and all contained components
	UFUNCTION(BlueprintCallable)
	virtual void ShopTick(EGamePhase Phase);

	// Refresh connections for all components around and on the provided position
	void RefreshAround(FGridPosition GridPos);

	ULogicalShop *GetShop() { return ShopParent; }

	EGamePlatform GetType() { return PlatformType; }

	void Resize(float Width, float Height);

	float GetFloorWidth() { return FloorWidth; }

	float GetFloorHeight() { return FloorHeight; }

	bool IsFloorStatic() { return bStaticFloor; }


#define CHECK_IN_LOP_BOUNDS(X, WIDTH) (X < 0 ? (X >= -((int32)WIDTH/2)) : (X <= ((int32)(WIDTH-1)/2)))

	// Check if this pos is within the confines of the platform.
	// Note: Width 10 means valid Xs are -5 to 4. 
	// Width 11 means valid Xs are -5 to 5
	bool IsGridPosValid(FGridPosition GridPos) { return CHECK_IN_LOP_BOUNDS(GridPos.X, FloorWidth) && CHECK_IN_LOP_BOUNDS(GridPos.Y, FloorHeight); }

#undef CHECK_IN_LOP_BOUNDS

};