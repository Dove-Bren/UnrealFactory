#pragma once

#include "CoreMinimal.h"

#include "GridPosition.generated.h"

USTRUCT(Blueprintable)
struct FGridPosition
{
	GENERATED_BODY()

	// X grid coordinate
	UPROPERTY(EditAnywhere)
	int32 X;

	// Y grid coordinate
	UPROPERTY(EditAnywhere)
	int32 Y;
};