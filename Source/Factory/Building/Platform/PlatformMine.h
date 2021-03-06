#pragma once

// Specialized platform for managing mine layers

#include "CoreMinimal.h"

#include "Platform.h"

#include "PlatformMine.generated.h"

UCLASS()
class UPlatformMine : public UPlatform
{
	GENERATED_BODY()
private:

	//// Wall parent component
	//UPROPERTY(EditAnywhere)
	//USceneComponent *Walls;

	//// Exposed first instance of the south wall for troubleshooting in the debugger
	//UPROPERTY(EditAnywhere)
	//UStaticMeshComponent *WallInst;

protected:


public:
	UPlatformMine();


};