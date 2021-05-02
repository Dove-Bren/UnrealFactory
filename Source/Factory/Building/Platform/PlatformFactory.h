#pragma once

// Specialized platform for managing factory layers

#include "CoreMinimal.h"

#include "Platform.h"

#include "PlatformFactory.generated.h"

#define MIN_FACTORY_WIDTH 10
#define MIN_FACTORY_HEIGHT 10


UCLASS()
class UPlatformFactory : public UPlatform
{
	GENERATED_BODY()
private:
	// Wall parent component
	UPROPERTY(EditAnywhere)
	USceneComponent *Walls;

	UStaticMesh *FloorMesh;
	UStaticMesh *WallMesh;
	UMaterial *FloorMat;

protected:

	// Current Width
	UPROPERTY(EditAnywhere)
	float FloorWidth;

	// Current Height
	UPROPERTY(EditAnywhere)
	float FloorHeight;

	void SpawnWalls(float Width, float Height);

public:
	UPlatformFactory();

	virtual void BeginPlay();

	void Resize(float Width, float Height);

};