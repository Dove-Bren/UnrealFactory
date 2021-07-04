#pragma once

// Super simple Map<EDirection, bool> but space efficient

#include "CoreMinimal.h"

#include "Factory/GameEnums.h"

#include "DirectionFlagMap.generated.h"

template<typename T>
struct FDirectionMap
{
	// Value for East
	T EastValue;

	// Value for South
	T SouthValue;

	// Value for West
	T WestValue;

	// Value for East
	T NorthValue;

	FDirectionMap() : FDirectionMap(T()) {};
	FDirectionMap(T bAll) : FDirectionMap(bAll, bAll, bAll, bAll) {}
	FDirectionMap(T East, T South, T West, T North) : EastValue(East), SouthValue(South), WestValue(West), NorthValue(North) {}
	FDirectionMap(const FDirectionMap<T> & Original) : FDirectionMap(Original.EastValue, Original.SouthValue, Original.WestValue, Original.NorthValue) {}

	void Rotate(bool bClockwise = true);
	FDirectionMap<T> RotateNew(bool bClockwise = true) const;

	T Get(EDirection Direction) const;
	void Set(EDirection, T Value);
};

template<typename T>
inline void FDirectionMap<T>::Rotate(bool bClockwise)
{
	T bOldEast = EastValue;
	if (bClockwise)
	{
		EastValue = (NorthValue);
		NorthValue = (WestValue);
		WestValue = (SouthValue);
		SouthValue = (bOldEast);
	}
	else
	{
		EastValue = (SouthValue);
		SouthValue = (WestValue);
		WestValue = (NorthValue);
		NorthValue = (bOldEast);
	}
	
}

template<typename T>
inline FDirectionMap<T> FDirectionMap<T>::RotateNew(bool bClockwise) const
{
	FDirectionMap Other = FDirectionMap(*this);
	Other.Rotate(bClockwise);
	return Other;
}

template<typename T>
inline T FDirectionMap<T>::Get(EDirection Direction) const
{
	T Value;
	switch (Direction)
	{
	case EDirection::EAST:
		Value = EastValue;
		break;
	case EDirection::SOUTH:
		Value = SouthValue;
		break;
	case EDirection::WEST:
		Value = WestValue;
		break;
	case EDirection::NORTH:
	default:
		Value = NorthValue;
		break;
	}
	return Value;
}

template<typename T>
inline void FDirectionMap<T>::Set(EDirection Direction, T Value)
{
	switch (Direction)
	{
	case EDirection::EAST:
		EastValue = Value;
		break;
	case EDirection::SOUTH:
		SouthValue = Value;
		break;
	case EDirection::WEST:
		WestValue = Value;
		break;
	case EDirection::NORTH:
	default:
		NorthValue = Value;
		break;
	}
}

// Wrapped up FDirectionMap<bool> that interacts with Unreal better
USTRUCT(Blueprintable)
struct FDirectionFlagMap
{
	GENERATED_BODY()

	FDirectionMap<bool> MapInternal;

	FDirectionFlagMap() : FDirectionFlagMap(false) {};
	FDirectionFlagMap(bool bAll) : FDirectionFlagMap(bAll, bAll, bAll, bAll) {}
	FDirectionFlagMap(bool bEast, bool bSouth, bool bWest, bool bNorth) { MapInternal = FDirectionMap<bool>(bEast, bSouth, bWest, bNorth); }
	FDirectionFlagMap(const FDirectionMap<bool> & OriginalMap) { MapInternal = OriginalMap; }
	FDirectionFlagMap(const FDirectionFlagMap & Original) : FDirectionFlagMap(Original.MapInternal.EastValue, Original.MapInternal.SouthValue, Original.MapInternal.WestValue, Original.MapInternal.NorthValue) {}

	void Rotate(bool bClockwise = true) { MapInternal.Rotate(bClockwise); };
	FDirectionFlagMap RotateNew(bool bClockwise = true) const { FDirectionFlagMap Other = FDirectionFlagMap(MapInternal); Other.Rotate(bClockwise); return Other; };

	bool Get(EDirection Direction) const { return MapInternal.Get(Direction); }
	void Set(EDirection Direction, bool bValue) { MapInternal.Set(Direction, bValue); }
};