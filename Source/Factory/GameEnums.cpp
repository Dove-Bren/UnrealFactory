#include "GameEnums.h"

FName EGamePlatformNames[] = {
	FName(TEXT("Shop")), // SHOP
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
