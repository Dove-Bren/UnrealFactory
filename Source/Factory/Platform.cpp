#include "Platform.h"


void UPlatform::AttachToShop(EGamePlatform Type, UShop *Shop)
{
	this->PlatformType = Type;
	this->ShopParent = Shop;
}

void UPlatform::AddComponent(APlatformComponent *Component)
{
	this->Components.Add(Component);
	Component->RegisterPlatform(this);
}

void UPlatform::StartPhase(EGamePhase Phase)
{
	for (APlatformComponent *comp : Components)
	{
		comp->StartPhase(Phase);
	}
}

void UPlatform::ShopTick(EGamePhase Phase)
{
	for (APlatformComponent *comp : Components)
	{
		comp->ShopTick(Phase);
	}
}