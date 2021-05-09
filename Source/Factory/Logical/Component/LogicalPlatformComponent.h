#pragma once

// A single component that exists on a platform.
// This is the base class, and includes placables as well as obstacles, etc.
// Basically anything that interacts with other components should be a component.

#include "CoreMinimal.h"

#include "Factory/GameEnums.h"

#include "LogicalPlatformComponent.generated.h"

typedef class ULogicalPlatform ULogicalPlatform;

UCLASS()
class ULogicalPlatformComponent : public UObject
{
	GENERATED_BODY()
private:

	// Owning Platform
	UPROPERTY(EditAnywhere)
	ULogicalPlatform *ParentPlatform;

protected:

public:

	ULogicalPlatformComponent();

	// Init the component as part of the provided platform
	UFUNCTION(BlueprintCallable)
	virtual void RegisterPlatform(ULogicalPlatform *Platform);

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

};