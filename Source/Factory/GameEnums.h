#pragma once

#include "CoreMinimal.h"
#include "Misc/EnumRange.h"

#include "GameEnums.generated.h"

#define ENUM_HAS_NAMES(Enum, ShortName) FName Enum##Names[];\
	inline FName Get##ShortName##Name(Enum PlatformType) { return Enum##Names[(uint8)PlatformType]; }

UENUM()
enum class EGamePlatform : uint8
{
	SHOP,
	FACTORY,
	MINE,

	MAX
};
ENUM_RANGE_BY_COUNT(EGamePlatform, EGamePlatform::MAX)
ENUM_HAS_NAMES(EGamePlatform, Platform);

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