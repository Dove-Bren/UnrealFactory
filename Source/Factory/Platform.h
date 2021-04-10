#pragma once

// A platform is a single vertical plane in the game. Each is mapped to a type (see GamePlatform).
// Platforms are responsible for managing the collections of all objects in the platform.
// Note that, for the Mine, this includes all of the machines belonging to one shop.

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

#include "GameEnums.h"
#include "PlatformComponent.h"

#include "Platform.generated.h"

typedef class AShop AShop;

UCLASS()
class UPlatform : public USceneComponent
{
	GENERATED_BODY()
private:

	// Main floor mesh of the platform
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *Mesh;

	// Type of platform
	UPROPERTY(VisibleAnywhere)
	EGamePlatform PlatformType;

	// Link to parent shop object
	UPROPERTY(VisibleAnywhere)
	AShop *ShopParent;

	// Collection of all components that are on this layer
	UPROPERTY(VisibleAnywhere)
	TArray<APlatformComponent*> Components;

protected:

	//std::vector

public:
	UPlatform();

	UFUNCTION(BlueprintCallable)
	void AttachToShop(EGamePlatform Type, AShop *Shop);

	// Add a component to this layer
	UFUNCTION(BlueprintCallable)
	void AddComponent(APlatformComponent *Component);

	// Transition to the provided state, including stopping any activities from the previous one
	UFUNCTION(BlueprintCallable)
	virtual void StartPhase(EGamePhase Phase);

	// Perform regular shop tick functions for the platform and all contained components
	UFUNCTION(BlueprintCallable)
	virtual void ShopTick(EGamePhase Phase);

};