#pragma once

// A platform is a single vertical plane in the game. Each is mapped to a type (see GamePlatform).
// Platforms are responsible for managing the collections of all objects in the platform.
// Note that, for the Mine, this includes all of the machines belonging to one shop.

#include "CoreMinimal.h"

#include "GameEnums.h"

#include "Platform.generated.h"

typedef class UShop UShop;

UCLASS()
class UPlatform : public UObject
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	EGamePlatform PlatformType;


	UPROPERTY(VisibleAnywhere)
	UShop *ShopParent;


	//TArray<Component*> Components

protected:

	//std::vector

public:
	UPlatform() {};

	UFUNCTION(BlueprintCallable)
	void AttachToShop(EGamePlatform Type, UShop *Shop);

	// Transition to the provided state, including stopping any activities from the previous one
	UFUNCTION(BlueprintCallable)
	void StartPhase(EGamePhase Phase);

};