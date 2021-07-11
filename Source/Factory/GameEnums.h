#pragma once

#include "CoreMinimal.h"
#include "Misc/EnumRange.h"

#include "GameEnums.generated.h"

#define ENUM_HAS_NAMES(Enum, ShortName) FName Enum##Names[];\
	inline FName Get##ShortName##Name(Enum PlatformType) { return Enum##Names[(uint8)PlatformType]; }

UENUM()
enum class EGamePlatform : uint8
{
	STORE,
	FACTORY,
	MINE,

	MAX
};
ENUM_RANGE_BY_COUNT(EGamePlatform, EGamePlatform::MAX)
ENUM_HAS_NAMES(EGamePlatform, Platform);

int32 GetPlatformOffset(EGamePlatform PlatformType);

UENUM()
enum class EGamePhase : uint8
{
	MORNING,
	AFTERNOON,
	EVENING,
	NIGHT,
	LATENIGHT,

	MAX
};
ENUM_RANGE_BY_COUNT(EGamePhase, EGamePhase::MAX)
ENUM_HAS_NAMES(EGamePhase, Phase);

UENUM()
enum class EDirection : uint8
{
	// Negative Y, Yaw = -90/270
	NORTH,

	// Positive X, Yaw = 0
	EAST,

	// Positive Y, Yaw = 90
	SOUTH,

	// Negative X, Yaw = 180
	WEST,

	MAX
};
ENUM_RANGE_BY_COUNT(EDirection, EDirection::MAX)
ENUM_HAS_NAMES(EDirection, Direction);

inline FRotator GetRotationFromDirection(EDirection Direction)
{
	FRotator Rotation(0, 0, 0);
	switch (Direction)
	{
	case EDirection::NORTH:
	default:
		Rotation.Yaw = 270;
		break;
	case EDirection::EAST:
		Rotation.Yaw = 0;
		break;
	case EDirection::SOUTH:
		Rotation.Yaw = 90;
		break;
	case EDirection::WEST:
		Rotation.Yaw = 180;
		break;
	}
	return Rotation;
};

inline EDirection GetDirectionFromYaw(float Yaw)
{
	EDirection Direction;
	while (Yaw < 0) Yaw += 360.0f;
	int idx = (FMath::RoundToInt(Yaw / 90.f) % 4); // 0 - 3
	switch (idx)
	{
	case 0:
		Direction = EDirection::EAST;
		break;
	case 1:
		Direction = EDirection::SOUTH;
		break;
	case 2:
		Direction = EDirection::WEST;
		break;
	case 3:
	default:
		Direction = EDirection::NORTH;
		break;
	}
	return Direction;
}

inline EDirection RotateDirection(EDirection Direction, bool bClockwise = true)
{
	EDirection Out;
	switch (Direction)
	{
	case EDirection::NORTH:
	default:
		Out = bClockwise ? EDirection::EAST : EDirection::WEST;
		break;
	case EDirection::EAST:
		Out = bClockwise ? EDirection::SOUTH : EDirection::NORTH;
		break;
	case EDirection::SOUTH:
		Out = bClockwise ? EDirection::WEST : EDirection::EAST;
		break;
	case EDirection::WEST:
		Out = bClockwise ? EDirection::NORTH : EDirection::SOUTH;
		break;
	}
	return Out;
}

inline EDirection OppositeDirection(EDirection Direction)
{
	EDirection Out;
	switch (Direction)
	{
	case EDirection::NORTH:
	default:
		Out = EDirection::SOUTH;
		break;
	case EDirection::EAST:
		Out = EDirection::WEST;
		break;
	case EDirection::SOUTH:
		Out = EDirection::NORTH;
		break;
	case EDirection::WEST:
		Out = EDirection::EAST;
		break;
	}
	return Out;
}

// Return a vector offset by magnitude 1 in the direction indicated in world-space
inline FVector GetDirectionOffset(EDirection Direction)
{
	FVector Vector(0, 0, 0);
	switch (Direction)
	{
	case EDirection::NORTH:
	default:
		Vector.Y = -1;
		break;
	case EDirection::EAST:
		Vector.X = 1;
		break;
	case EDirection::SOUTH:
		Vector.Y = 1;
		break;
	case EDirection::WEST:
		Vector.X = -1;
		break;
	}
	return Vector;
}

UENUM()
enum class EItemActionType : uint8
{
	NO_ACTION,
	USE,
	PLACE,
	EQUIP,

	MAX
};
ENUM_RANGE_BY_COUNT(EItemActionType, EItemActionType::MAX)
ENUM_HAS_NAMES(EItemActionType, ItemActionType);

#define COLOR_MODS(color) LIGHT_##color,\
	DARK_##color

UENUM()
enum class EStandardColors : uint8
{
	WHITE,
	BLACK,

	GRAY,
	LIGHT_GRAY,
	DARK_GRAY,

	RED,
	LIGHT_RED,
	DARK_RED,

	ORANGE,
	LIGHT_ORANGE,
	DARK_ORANGE,

	YELLOW,
	LIGHT_YELLOW,
	DARK_YELLOW,

	GREEN,
	LIGHT_GREEN,
	DARK_GREEN,

	CYAN,

	BLUE,
	LIGHT_BLUE,
	DARK_BLUE,

	PURPLE,
	LIGHT_PURPLE,
	DARK_PURPLE,

	FUCHSIA,

	BROWN,
	   
	MAX, // Enum stops here

	PINK = LIGHT_RED,
};
ENUM_RANGE_BY_COUNT(EStandardColors, EStandardColors::MAX)
ENUM_HAS_NAMES(EStandardColors, StandardColors);

// Return the integer value representation of the number.
// This is NOT RGB or ARGB or anything and instead is a specific
// index for materials.
uint8 GetColorIndex(EStandardColors Color);

// Return the color as an ARGB int.
uint32 GetColorARGB(EStandardColors Color);

// Return the color as an RGBA int.
uint32 GetColorRGBA(EStandardColors Color);

inline uint8 GetColorIndex(EStandardColors Color)
{
	// These numbers must be kept in sync with the Material Function "MF_Helper_GetColorFromIdx"
	switch (Color)
	{
	case EStandardColors::WHITE:
	default:
		return 1;
	case EStandardColors::BLACK:
		return 2;
	case EStandardColors::GRAY:
		return 3;
	case EStandardColors::LIGHT_GRAY:
		return 4;
	case EStandardColors::DARK_GRAY:
		return 5;
	case EStandardColors::RED:
		return 6;
	case EStandardColors::LIGHT_RED:
		return 7;
	case EStandardColors::DARK_RED:
		return 8;
	case EStandardColors::ORANGE:
		return 9;
	case EStandardColors::LIGHT_ORANGE:
		return 10;
	case EStandardColors::DARK_ORANGE:
		return 11;
	case EStandardColors::YELLOW:
		return 12;
	case EStandardColors::LIGHT_YELLOW:
		return 13;
	case EStandardColors::DARK_YELLOW:
		return 14;
	case EStandardColors::GREEN:
		return 15;
	case EStandardColors::LIGHT_GREEN:
		return 16;
	case EStandardColors::DARK_GREEN:
		return 17;
	case EStandardColors::BLUE:
		return 18;
	case EStandardColors::LIGHT_BLUE:
		return 19;
	case EStandardColors::DARK_BLUE:
		return 20;
	case EStandardColors::CYAN:
		return 21;
	case EStandardColors::PURPLE:
		return 22;
	case EStandardColors::LIGHT_PURPLE:
		return 23;
	case EStandardColors::DARK_PURPLE:
		return 24;
	case EStandardColors::FUCHSIA:
		return 25;
	case EStandardColors::BROWN:
		return 26;
	}
}

inline uint32 GetColorARGB(EStandardColors Color)
{
	switch (Color)
	{
	case EStandardColors::WHITE:
	default:
		return 0xFFFFFFFF;
	case EStandardColors::BLACK:
		return 0xFF000000;
	case EStandardColors::GRAY:
		return 0xFF7F7F7F;
	case EStandardColors::LIGHT_GRAY:
		return 0xFF3F3F3F;
	case EStandardColors::DARK_GRAY:
		return 0xFFBFBFBF;
	case EStandardColors::RED:
		return 0xFFFF0000;
	case EStandardColors::LIGHT_RED:
		return 0xFFFF80A0;
	case EStandardColors::DARK_RED:
		return 0xFFA00000;
	case EStandardColors::ORANGE:
		return 0xFFFF7000;
	case EStandardColors::LIGHT_ORANGE:
		return 0xFFFFA366;
	case EStandardColors::DARK_ORANGE:
		return 0xFFCD5930;
	case EStandardColors::YELLOW:
		return 0xFFFFFF00;
	case EStandardColors::LIGHT_YELLOW:
		return 0xFFFFFF80;
	case EStandardColors::DARK_YELLOW:
		return 0xFFB5B524;
	case EStandardColors::GREEN:
		return 0xFF00FF00;
	case EStandardColors::LIGHT_GREEN:
		return 0xFF80FF60;
	case EStandardColors::DARK_GREEN:
		return 0xFF008010;
	case EStandardColors::BLUE:
		return 0xFF0000FF;
	case EStandardColors::LIGHT_BLUE:
		return 0xFFA0D0FF;
	case EStandardColors::DARK_BLUE:
		return 0xFF050180;
	case EStandardColors::CYAN:
		return 0xFF00FFFF;
	case EStandardColors::PURPLE:
		return 0xFF8000FF;
	case EStandardColors::LIGHT_PURPLE:
		return 0xFFC070FF;
	case EStandardColors::DARK_PURPLE:
		return 0xFF340699;
	case EStandardColors::FUCHSIA:
		return 0xFFCA2C92;
	case EStandardColors::BROWN:
		return 0xFF70360B;
	}
}

inline uint32 GetColorRGBA(EStandardColors Color)
{
	switch (Color)
	{
	case EStandardColors::WHITE:
	default:
		return 0xFFFFFFFF;
	case EStandardColors::BLACK:
		return 0x000000FF;
	case EStandardColors::GRAY:
		return 0x7F7F7FFF;
	case EStandardColors::LIGHT_GRAY:
		return 0x3F3F3FFF;
	case EStandardColors::DARK_GRAY:
		return 0xBFBFBFFF;
	case EStandardColors::RED:
		return 0xFF0000FF;
	case EStandardColors::LIGHT_RED:
		return 0xFF80A0FF;
	case EStandardColors::DARK_RED:
		return 0xA00000FF;
	case EStandardColors::ORANGE:
		return 0xFF7000FF;
	case EStandardColors::LIGHT_ORANGE:
		return 0xFFA366FF;
	case EStandardColors::DARK_ORANGE:
		return 0xCD5930FF;
	case EStandardColors::YELLOW:
		return 0xFFFF00FF;
	case EStandardColors::LIGHT_YELLOW:
		return 0xFFFF80FF;
	case EStandardColors::DARK_YELLOW:
		return 0xB5B524FF;
	case EStandardColors::GREEN:
		return 0x00FF00FF;
	case EStandardColors::LIGHT_GREEN:
		return 0x80FF60FF;
	case EStandardColors::DARK_GREEN:
		return 0x008010FF;
	case EStandardColors::BLUE:
		return 0x0000FFFF;
	case EStandardColors::LIGHT_BLUE:
		return 0xA0D0FFFF;
	case EStandardColors::DARK_BLUE:
		return 0x050180FF;
	case EStandardColors::CYAN:
		return 0x00FFFFFF;
	case EStandardColors::PURPLE:
		return 0x8000FFFF;
	case EStandardColors::LIGHT_PURPLE:
		return 0xC070FFFF;
	case EStandardColors::DARK_PURPLE:
		return 0x340699FF;
	case EStandardColors::FUCHSIA:
		return 0xCA2C92FF;
	case EStandardColors::BROWN:
		return 0x70360BFF;
	}
}

UENUM()
enum class EConnectionStatus : uint8
{
	NO_CONNECTION,
	VALID,
	ACTIVE = VALID,
	INVALID,
	INACTIVE = INVALID,

	MAX
};
ENUM_RANGE_BY_COUNT(EConnectionStatus, EConnectionStatus::MAX)

UENUM()
enum class EResourceType : uint8
{
	GOLD,
	WOOD,
	ORE,

	MAX
};
ENUM_RANGE_BY_COUNT(EResourceType, EResourceType::MAX)
ENUM_HAS_NAMES(EResourceType, ResourceType);
