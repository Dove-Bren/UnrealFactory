#include "Platform.h"


void UPlatform::AttachToShop(EGamePlatform Type, UShop *Shop)
{
	this->PlatformType = Type;
	this->ShopParent = Shop;
}

void UPlatform::StartPhase(EGamePhase Phase)
{
	;
}