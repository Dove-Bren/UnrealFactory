#include "LogicalPlatformComponent.h"

ULogicalPlatformComponent::ULogicalPlatformComponent()
{
	;
}

void ULogicalPlatformComponent::RegisterPlatform(ULogicalPlatform * Platform)
{
	this->ParentPlatform = Platform;
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
