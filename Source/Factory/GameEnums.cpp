#include "GameEnums.h"

#define SPACE_BETWEEN_PLATFORMS 100 * 20

FName EGamePlatformNames[] = {
	FName(TEXT("Shop")), // SHOP
	FName(TEXT("Factory")), // FACTORY
	FName(TEXT("Mine")), // MINE
};
static_assert(sizeof(EGamePlatformNames) / sizeof(EGamePlatformNames[0]) == (uint8)EGamePlatform::MAX, "Platform name list not kept in sync");

int32 GetPlatformOffset(EGamePlatform PlatformType)
{
	switch (PlatformType)
	{
	case EGamePlatform::SHOP:
		return SPACE_BETWEEN_PLATFORMS * 2;
	case EGamePlatform::FACTORY:
		return SPACE_BETWEEN_PLATFORMS;
	case EGamePlatform::MINE:
		return 0;
	}

	return 0;
}


FName EGamePhaseNames[] = {
	FName(TEXT("Morning")), // MORNING
	FName(TEXT("Afternoon")), // AFTERNOON
	FName(TEXT("Evening")), // EVENING
	FName(TEXT("Night")), // NIGHT
	FName(TEXT("Late Night")), //LATENIGHT
};
static_assert(sizeof(EGamePhaseNames) / sizeof(EGamePhaseNames[0]) == (uint8)EGamePhase::MAX, "Phase name list not kept in sync");


FName EDirectionNames[] = {
	FName(TEXT("North")), // NORTH
	FName(TEXT("East")), // EAST
	FName(TEXT("South")), // SOUTH
	FName(TEXT("West")), // WEST
};
static_assert(sizeof(EDirectionNames) / sizeof(EDirectionNames[0]) == (uint8)EDirection::MAX, "Phase name list not kept in sync");
