#include "LogicalBelt.h"

/*static*/ ULogicalBelt *ULogicalBelt::MakeBelt(EDirection Direction)
{
	ULogicalBelt *Belt = NewObject<ULogicalBelt>();
	Belt->SetDirection(Direction);
	return Belt;
}

void ULogicalBelt::RegisterPlatform(ULogicalPlatform *Platform, FGridPosition GridPosition)
{
	Super::RegisterPlatform(Platform, GridPosition);

	; // Maybe at some point we'll wnat to track all the belts specifically
}

void ULogicalBelt::RemoveFromPlatform(ULogicalPlatform *Platform)
{
	Super::RemoveFromPlatform(Platform);

	;
}

bool ULogicalBelt::IsActiveDuring(EGamePhase Phase)
{
	return true;
}

void ULogicalBelt::ShopTick(EGamePhase Phase)
{
	; // TODO do belt mechs!
}

void ULogicalBelt::RefreshNearby(FLocalLayout NearbyLayout)
{
	; // TODO find nearby things and create connections!
}