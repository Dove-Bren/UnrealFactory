#pragma once

// A single component that exists on a platform.
// This is the base class, and includes placables as well as obstacles, etc.
// Basically anything that interacts with other components should be a component.

#include <functional>

#include "CoreMinimal.h"

#include "Factory/DirectionFlagMap.h"
#include "Factory/GameEnums.h"
#include "Factory/GridPosition.h"
#include "LocalLayout.h"

#include "LogicalPlatformComponent.generated.h"

typedef struct ClickOption ClickOption;

typedef class ULogicalPlatform ULogicalPlatform;
typedef class APlayerCharacter APlayerCharacter;
typedef class UItem UItem;
typedef class UItemType UItemType;

typedef class APlatformComponent APlatformComponent;
typedef class UPlatform UPlatform;

UCLASS(BlueprintType, Abstract)
class ULogicalPlatformComponent : public UObject
{
	GENERATED_BODY()
private:

	// Owning Platform
	UPROPERTY(VisibleInstanceOnly)
	ULogicalPlatform *ParentPlatform;

	// World actor spawned for this component, if there is one.
	// This can easily become null as the player moves around. Check before using!
	UPROPERTY(VisibleInstanceOnly)
	APlatformComponent *WorldActor;

protected:

	// Current grid position of the component
	UPROPERTY(VisibleInstanceOnly)
	FGridPosition Position;

	// Current direction
	UPROPERTY(VisibleInstanceOnly)
	EDirection Direction = EDirection::NORTH;
	
	// Whether this component can be removed with a click action.
	// Can be set in an archtype to set whether it's ever possible.
	// May be used on instances at runtime to temporarily disable the remove option.
	// If setting to true, consider setting up an item to recover as RemoveItemType.
	UPROPERTY(EditAnywhere)
	bool bRemoveable;

	// Whether this component can be clicked and 'used'.
	// If set, the component must also override the OnUse method to handle the
	// interact attempt.
	// Can be set in an archtype to set whether it's ever possible.
	// May be used on instances at runtime to temporarily disable the remove option.
	UPROPERTY(EditAnywhere)
	bool bUsable;

	//// If set, this component should be 'moveable' through a click action.
	//// Can be set in an archtype to set whether it's ever possible.
	//// May be used on instances at runtime to temporarily disable the remove option.
	//UPROPERTY(EditAnywhere)
	//bool bMoveable;
	
	// If set, this component can be rotated without picking it back up first.
	// Can be set in an archtype to set whether it's ever possible.
	// May be used on instances at runtime to temporarily disable the remove option.
	UPROPERTY(EditAnywhere)
	bool bRotatable;

	// What item to give back to the player if this component is removed.
	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemType> RemoveItemType;

	// What color to highlight this component if it's close enough to be interacted with.
	UPROPERTY(EditAnywhere)
	EStandardColors HighlightColor;

	// What menu to use to show all available actions when this component is clicked.
	TSubclassOf<class UPopupMenuWidget> PopupWidgetClass;

	// What item to give back to the player if this component is removed.
	UFUNCTION(BlueprintCallable)
	virtual bool OnUse(APlayerCharacter *Player); // TODO consider making a BP event

	static FActorSpawnParameters MakeSpawnParams() { FActorSpawnParameters Params; Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; return Params; }
	void MakeSpawnLocation(FVector &Location);
	// should store direction!

	FLocalLayout & FixupLocalLayout(FLocalLayout & Existing);

	APlatformComponent *GetWorldActor();

	virtual void OnRotation(EDirection OldDirection, EDirection NewDirection);

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

	UFUNCTION(BlueprintCallable)
	FGridPosition GetPosition() const { return Position; }

	UFUNCTION(BlueprintCallable)
	EDirection GetDirection() const { return Direction; }

	APlatformComponent *SpawnWorldComponent(UPlatform *Platform);

	virtual APlatformComponent *SpawnWorldComponentInternal(UPlatform *Platform) PURE_VIRTUAL(ULogicalPlatformComponent::SpawnWorldComponent, return nullptr;);

	virtual void RefreshWorldActor();

	UFUNCTION(BlueprintCallable)
	ULogicalPlatform *GetParentPlatform() { return ParentPlatform; }

	// Check whether an actor representing this component should highlight themselves
	// if hovered with the mouse.
	virtual bool ShouldHighlight(APlayerCharacter *Player, float Distance);

	// Get what color the highlight should be
	virtual EStandardColors GetHighlightColor(APlayerCharacter *Player, float Distance);

	virtual bool GetClickOptions(APlayerCharacter *Player, float Distance, FVector ActorLocation, ClickOption **DefaultOptOut, TArray<ClickOption> *OptionsOut);

	virtual TSubclassOf<class UPopupMenuWidget> GetMenuWidgetClass() { return PopupWidgetClass; }

};

typedef struct ClickOption
{
	FName Name;
	std::function<void()> OnClick;

	ClickOption(FName Name, std::function<void()> OnClick) : Name(Name), OnClick(OnClick) {}
} ClickOption;