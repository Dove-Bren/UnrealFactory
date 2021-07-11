#pragma once

// Information about components that are immediate neighbors

#include "CoreMinimal.h"

#include "Factory/GameEnums.h"

#include "LocalLayout.generated.h"

typedef class ULogicalPlatformComponent ULogicalPlatformComponent;

USTRUCT(BlueprintType)
struct FLocalLayout
{
	GENERATED_BODY()

	ULogicalPlatformComponent *Center = nullptr;
	ULogicalPlatformComponent *North = nullptr;
	ULogicalPlatformComponent *East = nullptr;
	ULogicalPlatformComponent *South = nullptr;
	ULogicalPlatformComponent *West = nullptr;

	ULogicalPlatformComponent *GetDirection(EDirection Direction) const;
};

inline ULogicalPlatformComponent *FLocalLayout::GetDirection(EDirection Direction) const
{
	ULogicalPlatformComponent *Comp;
	switch (Direction)
	{
	case EDirection::EAST:
		Comp = East;
		break;
	case EDirection::SOUTH:
		Comp = South;
		break;
	case EDirection::WEST:
		Comp = West;
		break;
	case EDirection::NORTH:
	default:
		Comp = North;
		break;
	}
	return Comp;
}