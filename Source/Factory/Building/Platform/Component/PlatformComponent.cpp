#include "PlatformComponent.h"

APlatformComponent::APlatformComponent()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	this->RootComponent = Mesh;
}

void APlatformComponent::RegisterPlatform(UPlatform * Platform)
{
	this->ParentPlatform = Platform;
}

void APlatformComponent::RemoveFromPlatform(UPlatform *Platform)
{
	this->ParentPlatform = nullptr;
}

void APlatformComponent::StartPhase(EGamePhase Phase)
{
	;
}

bool APlatformComponent::IsActiveDuring(EGamePhase Phase)
{
	return true;
}

void APlatformComponent::ShopTick(EGamePhase Phase)
{
	;
}
