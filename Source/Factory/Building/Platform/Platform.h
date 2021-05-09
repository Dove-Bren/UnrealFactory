#pragma once

// A platform is a single vertical plane in the game. Each is mapped to a type (see GamePlatform).
// Platforms are responsible for managing the collections of all objects in the platform.
// Note that, for the Mine, this includes all of the machines belonging to one shop.

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Factory/GameEnums.h"
#include "Factory/Logical/LogicalPlatform.h"
#include "NonComponents/Ladder.h"

#include "Platform.generated.h"

typedef class AShop AShop;

UCLASS()
class UPlatform : public USceneComponent
{
	GENERATED_BODY()
private:

	// Link to logical platform
	UPROPERTY(VisibleAnywhere)
	ULogicalPlatform *LogicalPlatform;

	// Link to logical platform
	UPROPERTY(VisibleAnywhere)
	EGamePlatform PlatformType;

protected:

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

	// Ladder
	UPROPERTY(VisibleAnywhere)
	ALadder *Ladder;

	void SpawnWalls(float Width, float Height);

public:
	UPlatform();
	virtual ~UPlatform() = default;

	void SetLogicalPlatform(ULogicalPlatform *Platform);

	UFUNCTION(BlueprintCallable)
	void AttachToShop(EGamePlatform Type, AShop *Shop);

	virtual void BeginPlay() override;

	AShop *GetShop() { return ShopParent; }

	ULogicalPlatform *GetLogicalPlatform() { return LogicalPlatform; }

	EGamePlatform GetType() { return PlatformType; }

	void RefreshFloor();

};