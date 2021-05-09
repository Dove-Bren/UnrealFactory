#pragma once

// Information about components that are immediate neighbors

#include "CoreMinimal.h"

#include "LocalLayout.generated.h"

typedef class ULogicalPlatformComponent ULogicalPlatformComponent;

USTRUCT(BlueprintType)
struct FLocalLayout
{
	GENERATED_BODY()

	ULogicalPlatformComponent *Center;
	ULogicalPlatformComponent *North;
	ULogicalPlatformComponent *East;
	ULogicalPlatformComponent *South;
	ULogicalPlatformComponent *West;
};