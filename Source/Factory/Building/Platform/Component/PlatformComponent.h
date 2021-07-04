#pragma once

// A single component that exists on a platform.
// This is the base class, and includes placables as well as obstacles, etc.
// Basically anything that interacts with other components should be a component.

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

#include "Factory/GameEnums.h"

#include "PlatformComponent.generated.h"

typedef class UPlatform UPlatform;

UCLASS()
class APlatformComponent : public AActor
{
	GENERATED_BODY()
private:

	// Owning Platform
	UPROPERTY(EditAnywhere)
	UPlatform *ParentPlatform;

protected:

	// Base mesh component
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *Mesh;

public:

	APlatformComponent();
	virtual ~APlatformComponent() = default;

	// TODO remove these as they are part of logical

	// Init the component as part of the provided platform
	UFUNCTION(BlueprintCallable)
	virtual void RegisterPlatform(UPlatform *Platform);

	// Detatch the component from the platform
	UFUNCTION(BlueprintCallable)
	virtual void RemoveFromPlatform(UPlatform *Platform);

	// Transition this component to the provided daily phase
	UFUNCTION(BlueprintCallable)
	virtual void StartPhase(EGamePhase Phase);

	// Check whether this component is active in the provided phase
	UFUNCTION(BlueprintCallable)
	virtual bool IsActiveDuring(EGamePhase Phase);

	// Perform regular shop tick functions for the provided phase
	UFUNCTION(BlueprintCallable)
	virtual void ShopTick(EGamePhase Phase);

	virtual void Refresh() {}

};