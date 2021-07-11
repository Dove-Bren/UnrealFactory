#pragma once

// A single component that exists on a platform.
// This is the base class, and includes placables as well as obstacles, etc.
// Basically anything that interacts with other components should be a component.

#include "CoreMinimal.h"

#include "Factory/DirectionFlagMap.h"
#include "Factory/GameEnums.h"
#include "Factory/GridPosition.h"
#include "LocalLayout.h"

#include "LogicalPlatformComponent.generated.h"

typedef class ULogicalPlatform ULogicalPlatform;
typedef class APlayerCharacter APlayerCharacter;
typedef class UItem UItem;

typedef class APlatformComponent APlatformComponent;
typedef class UPlatform UPlatform;

UCLASS(BlueprintType, Abstract)
class ULogicalPlatformComponent : public UObject
{
	GENERATED_BODY()
private:

	// Owning Platform
	UPROPERTY(EditAnywhere)
	ULogicalPlatform *ParentPlatform;

	// World actor spawned for this component, if there is one.
	// This can easily become null as the player moves around. Check before using!
	APlatformComponent *WorldActor;

protected:

	UPROPERTY(VisibleAnywhere)
	FGridPosition Position;

	UPROPERTY(VisibleAnywhere)
	EDirection Direction = EDirection::NORTH;

	static FActorSpawnParameters MakeSpawnParams() { FActorSpawnParameters Params; Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; return Params; }
	void MakeSpawnLocation(FVector &Location);
	// should store direction!

	FLocalLayout & FixupLocalLayout(FLocalLayout & Existing);

	APlatformComponent *GetWorldActor();

public:

	ULogicalPlatformComponent();
	virtual ~ULogicalPlatformComponent() = default;

	// Called when the component is first ever placed with an item.
	// Note this is NOT called when loading and recreating things from save data.
	// This is done BEFORE registering component to the platform, so platform and position will not be valid.
	virtual void OnInitialPlacement(APlayerCharacter *PlacingCharacter, UItem *Item, EDirection DirectionIn) { this->Direction = DirectionIn; };

	// Init the component as part of the provided platform
	UFUNCTION(BlueprintCallable)
	virtual void RegisterPlatform(ULogicalPlatform *Platform, FGridPosition GridPosition);

	// Detatch the component from the platform
	UFUNCTION(BlueprintCallable)
	virtual void RemoveFromPlatform(ULogicalPlatform *Platform);

	// Transition this component to the provided daily phase
	UFUNCTION(BlueprintCallable)
	virtual void StartPhase(EGamePhase Phase);

	// Check whether this component is active in the provided phase
	UFUNCTION(BlueprintCallable)
	virtual bool IsActiveDuring(EGamePhase Phase);

	// Perform regular shop tick functions for the provided phase
	UFUNCTION(BlueprintCallable)
	virtual void ShopTick(EGamePhase Phase);

	// Refresh information about what this component is next to and connected to
	// Returns true if the component has changed (indicating any world actors may need to be updated, too)
	UFUNCTION(BlueprintCallable)
	bool RefreshConnections();

	// Get what directions we'd accept incoming connections from IF WE ARE FACING EAST.
	// This function is called on the CDO and can't do anything fancy. Be warned!
	virtual FDirectionFlagMap GetDefaultIncomingConnectionPorts() { return FDirectionFlagMap(); }
	virtual FDirectionFlagMap GetDefaultOutgoingConnectionPorts() { return FDirectionFlagMap(); }
	
	// Not to be called on a CDO. Does the above but rotates for our given direction.
	virtual FDirectionFlagMap GetIncomingConnectionPorts();
	virtual FDirectionFlagMap GetOutgoingConnectionPorts();

	virtual bool RefreshNearby(FLocalLayout NearbyLayout = { });

	FGridPosition GetPosition() const { return Position; }

	EDirection GetDirection() const { return Direction; }

	APlatformComponent *SpawnWorldComponent(UPlatform *Platform);

	virtual APlatformComponent *SpawnWorldComponentInternal(UPlatform *Platform) PURE_VIRTUAL(ULogicalPlatformComponent::SpawnWorldComponent, return nullptr;);

	virtual void RefreshWorldActor();

	ULogicalPlatform *GetParentPlatform() { return ParentPlatform; }

};