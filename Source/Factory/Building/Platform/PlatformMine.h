#pragma once

// Specialized platform for managing mine layers

#include "CoreMinimal.h"

#include "Platform.h"

#include "PlatformMine.generated.h"

#define MINE_FLOOR_WIDTH 256
#define MINE_FLOOR_HEIGHT 256


UCLASS()
class UPlatformMine : public UPlatform
{
	GENERATED_BODY()
private:

	// Wall parent component
	UPROPERTY(EditAnywhere)
	USceneComponent *Walls;

	// Exposed first instance of the south wall for troubleshooting in the debugger
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *WallInst;

protected:


public:
	UPlatformMine();


};