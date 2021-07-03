#include "LogicalPlatformComponent.h"

#include "Factory/Logical/LogicalPlatform.h"

ULogicalPlatformComponent::ULogicalPlatformComponent()
{
	;
}

void ULogicalPlatformComponent::RegisterPlatform(ULogicalPlatform * Platform, FGridPosition GridPosition)
{
	this->ParentPlatform = Platform;
	this->Position = GridPosition;
}

void ULogicalPlatformComponent::RemoveFromPlatform(ULogicalPlatform *Platform)
{
	this->ParentPlatform = nullptr;
}

void ULogicalPlatformComponent::StartPhase(EGamePhase Phase)
{
	;
}

bool ULogicalPlatformComponent::IsActiveDuring(EGamePhase Phase)
{
	return true;
}

void ULogicalPlatformComponent::ShopTick(EGamePhase Phase)
{
	;
}

void ULogicalPlatformComponent::RefreshConnections()
{
	// TODO gather nearby layout
	FLocalLayout Layout{};

	if (ParentPlatform)
	{
		Layout = ParentPlatform->GetComponent(Position);
	}

	RefreshNearby(Layout);
}

void ULogicalPlatformComponent::RefreshNearby(FLocalLayout NearbyLayout)
{
	;
}

void ULogicalPlatformComponent::MakeSpawnLocation(FVector &Location)
{
	if (!ParentPlatform)
	{
		Location.X = Location.Y = Location.Z = 0;
	}
	else
	{
		Location = ParentPlatform->GetWorldPosFromGrid(Position, true);
	}
}