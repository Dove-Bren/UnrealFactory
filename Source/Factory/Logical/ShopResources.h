#pragma once

#include "CoreMinimal.h"

#include "Factory/GameEnums.h"

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

	int32 GetCount(EResourceType Type) const;
};

inline int32 FShopResources::GetCount(EResourceType Type) const
{
	switch (Type)
	{
	case EResourceType::GOLD:
	default:
		return this->Gold;
	case EResourceType::WOOD:
		return this->Wood;
	case EResourceType::ORE:
		return this->Ore;
	}
}