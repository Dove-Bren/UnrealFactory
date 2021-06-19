#pragma once

#include "CoreMinimal.h"

#include "ShopResources.generated.h"

USTRUCT(BlueprintType)
struct FShopResources
{
	GENERATED_BODY()

	// Gold. Fluid currency.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Gold;

	// Wood. Building resource and material resource.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Wood;

	// Raw ore. Base material resource.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Ore;
};