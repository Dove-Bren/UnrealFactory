#pragma once

#include "CoreMinimal.h"

#include "Factory/GameEnums.h"

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

	FGridPosition() : FGridPosition(0, 0) {};
	FGridPosition(int32 PosX, int32 PosY) : X(PosX), Y(PosY) {}

	friend bool operator==(const FGridPosition& Pos1, const FGridPosition& Pos2) { return (Pos1.X == Pos2.X && Pos1.Y == Pos2.Y); }
	friend bool operator!=(const FGridPosition& Pos1, const FGridPosition& Pos2) { return !(Pos1 == Pos2); }

	FGridPosition Offset(EDirection Direction);
	FGridPosition Move(EDirection Direction);
};

inline FGridPosition FGridPosition::Offset(EDirection Direction)
{
	switch (Direction)
	{
	case EDirection::NORTH:
	default:
		return FGridPosition(X, Y - 1);
	case EDirection::SOUTH:
		return FGridPosition(X, Y + 1);
	case EDirection::EAST:
		return FGridPosition(X + 1, Y);
	case EDirection::WEST:
		return FGridPosition(X - 1, Y);
	}
}

inline FGridPosition FGridPosition::Move(EDirection Direction)
{
	switch (Direction)
	{
	case EDirection::NORTH:
		Y--;
		break;
	case EDirection::SOUTH:
		Y++;
		break;
	case EDirection::EAST:
		X++;
		break;
	case EDirection::WEST:
		X--;
		break;
	}

	return *this;
}