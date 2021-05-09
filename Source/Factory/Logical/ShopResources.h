#pragma once

#include "CoreMinimal.h"

#include "ShopResources.generated.h"

USTRUCT(BlueprintType)
struct FShopResources
{
	GENERATED_BODY()

	uint32 Gold;
	uint32 Wood;
	uint32 Ore;
};