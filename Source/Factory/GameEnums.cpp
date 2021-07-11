#include "GameEnums.h"

#define SPACE_BETWEEN_PLATFORMS 100 * 20

FName EGamePlatformNames[] = {
	FName(TEXT("Store")), // STORE
	FName(TEXT("Factory")), // FACTORY
	FName(TEXT("Mine")), // MINE
};
static_assert(sizeof(EGamePlatformNames) / sizeof(EGamePlatformNames[0]) == (uint8)EGamePlatform::MAX, "Platform name list not kept in sync");


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


FName EItemActionTypeNames[] = {
	FName(TEXT("No Action")), // NO_ACTION
	FName(TEXT("Use")), // USE
	FName(TEXT("Place")), // PLACE
	FName(TEXT("Equip")), // EQUIP
};
static_assert(sizeof(EItemActionTypeNames) / sizeof(EItemActionTypeNames[0]) == (uint8)EItemActionType::MAX, "Item Action Type name list not kept in sync");

FName EStandardColorsNames[] = {

	FName(TEXT("White")), // WHITE,
	FName(TEXT("Black")), // BLACK,

	FName(TEXT("Gray")), // GRAY,
	FName(TEXT("Light Gray")), // LIGHT_GRAY,
	FName(TEXT("Dark Gray")), // DARK_GRAY,

	FName(TEXT("Red")), // RED,
	FName(TEXT("Pink")), // LIGHT_RED,
	FName(TEXT("Dark Red")), // DARK_RED,

	FName(TEXT("Orange")), // ORANGE,
	FName(TEXT("Light Orange")), // LIGHT_ORANGE,
	FName(TEXT("Dark Orange")), // DARK_ORANGE,

	FName(TEXT("Yellow")), // YELLOW,
	FName(TEXT("Light Yellow")), // LIGHT_YELLOW,
	FName(TEXT("Dark Yellow")), // DARK_YELLOW,

	FName(TEXT("Green")), // GREEN,
	FName(TEXT("Light Green")), // LIGHT_GREEN,
	FName(TEXT("Dark Green")), // DARK_GREEN,

	FName(TEXT("Blue")), // BLUE,
	FName(TEXT("Light Blue")), // LIGHT_BLUE,
	FName(TEXT("Dark Blue")), // DARK_BLUE,

	FName(TEXT("Cyan")), // CYAN,

	FName(TEXT("Purple")), // PURPLE,
	FName(TEXT("Light Purple")), // LIGHT_PURPLE,
	FName(TEXT("Dark Purple")), // DARK_PURPLE,

	FName(TEXT("Fuchsia")), // FUCHSIA,

	FName(TEXT("Brown")), // BROWN,
};
static_assert(sizeof(EStandardColorsNames) / sizeof(EStandardColorsNames[0]) == (uint8)EStandardColors::MAX, "Standard Color name list not kept in sync");

int32 GetPlatformOffset(EGamePlatform PlatformType)
{
	switch (PlatformType)
	{
	case EGamePlatform::STORE:
		return SPACE_BETWEEN_PLATFORMS * 2;
	case EGamePlatform::FACTORY:
		return SPACE_BETWEEN_PLATFORMS;
	case EGamePlatform::MINE:
		return 0;
	}

	return 0;
}

FName EResourceTypeNames[] = {
	FName(TEXT("Gold")), // GOLD
	FName(TEXT("Wood")), // WOOD
	FName(TEXT("Ore")), // ORE
};
static_assert(sizeof(EResourceTypeNames) / sizeof(EResourceTypeNames[0]) == (uint8)EResourceType::MAX, "Resource name list not kept in sync");