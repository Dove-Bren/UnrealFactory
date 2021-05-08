#pragma once

// A platform is a single vertical plane in the game. Each is mapped to a type (see GamePlatform).
// Platforms are responsible for managing the collections of all objects in the platform.
// Note that, for the Mine, this includes all of the machines belonging to one shop.

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Factory/GameEnums.h"
#include "Component/ComponentLayout.h"
#include "Component/PlatformComponent.h"
#include "NonComponents/Ladder.h"

#include "Platform.generated.h"

#define CELL_SIZE 100
#define MIN_PLATFORM_WIDTH 10
#define MIN_PLATFORM_HEIGHT 10

typedef class AShop AShop;

UCLASS()
class UPlatform : public USceneComponent
{
	GENERATED_BODY()
private:

	// Type of platform
	UPROPERTY(VisibleAnywhere)
	EGamePlatform PlatformType;

protected:

	//std::vector

	// Main floor mesh of the platform
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *Mesh;

	// Wall parent component
	UPROPERTY(EditAnywhere)
	USceneComponent *WallsParent;

	TArray<UStaticMeshComponent*> WallComponents;

	UStaticMesh *FloorMesh;
	UStaticMesh *WallMesh;
	UMaterial *FloorMat;

	// Link to parent shop object
	UPROPERTY(VisibleAnywhere)
	AShop *ShopParent;

	// Grid
	UPROPERTY(VisibleAnywhere)
	UComponentLayout *Grid;

	// Ladder
	UPROPERTY(VisibleAnywhere)
	ALadder *Ladder;

	// If true, prevents floor resizing and dynamic walls
	bool bStaticFloor = false;

	// Current Width
	UPROPERTY(EditAnywhere)
	float FloorWidth;

	// Current Height
	UPROPERTY(EditAnywhere)
	float FloorHeight;

	void SpawnWalls(float Width, float Height);

public:
	UPlatform();

	virtual ~UPlatform() = default;

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

	virtual void BeginPlay() override;

	const AShop *GetShop() { return ShopParent; }

	EGamePlatform GetType() { return PlatformType; }

	void Resize(float Width, float Height);

};