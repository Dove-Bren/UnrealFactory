#include "LogicalPlatformComponent.h"

#include "Factory/Logical/LogicalPlatform.h"

#include "Factory/Building/Platform/Component/PlatformComponent.h"

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

bool ULogicalPlatformComponent::RefreshConnections()
{
	// TODO gather nearby layout
	FLocalLayout Layout{};

	return RefreshNearby(FixupLocalLayout(Layout));
}

bool ULogicalPlatformComponent::RefreshNearby(FLocalLayout NearbyLayout)
{
	return false;
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

FLocalLayout & ULogicalPlatformComponent::FixupLocalLayout(FLocalLayout & Existing)
{
	if (!Existing.Center && ParentPlatform)
	{
		Existing = ParentPlatform->GetComponent(Position);
	}
	return Existing;
}

APlatformComponent *ULogicalPlatformComponent::GetWorldActor()
{
	if (WorldActor && !WorldActor->IsValidLowLevel())
	{
		WorldActor = nullptr;
	}
	return WorldActor;
}

void ULogicalPlatformComponent::RefreshWorldActor()
{
	APlatformComponent *Actor = GetWorldActor(); // Not using variable to give chance to clear if it's been destroyed
	if (Actor)
	{
		Actor->Refresh();
	}
}

FDirectionFlagMap ULogicalPlatformComponent::GetIncomingConnectionPorts()
{
	FDirectionFlagMap Map = this->GetDefaultIncomingConnectionPorts();
	EDirection DirIter = EDirection::EAST;
	while (DirIter != GetDirection())
	{
		DirIter = RotateDirection(DirIter);
		Map.Rotate();
	}
	return Map;
}

FDirectionFlagMap ULogicalPlatformComponent::GetOutgoingConnectionPorts()
{
	FDirectionFlagMap Map = this->GetDefaultOutgoingConnectionPorts();
	EDirection DirIter = EDirection::EAST;
	while (DirIter != GetDirection())
	{
		DirIter = RotateDirection(DirIter);
		Map.Rotate();
	}
	return Map;
}

APlatformComponent *ULogicalPlatformComponent::SpawnWorldComponent(UPlatform *Platform)
{
	APlatformComponent *Actor = this->SpawnWorldComponentInternal(Platform);
	this->WorldActor = Actor;
	return Actor;
}