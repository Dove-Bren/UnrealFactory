#include "LogicalPlatformFactory.h"

#include "Factory/Logical/LogicalShop.h"

ULogicalPlatformFactory::ULogicalPlatformFactory()
{
	;
}

void ULogicalPlatformFactory::ShopTick(EGamePhase Phase)
{
	Super::ShopTick(Phase);

	if (ShopParent)
	{
		ShopParent->AddWood(1);
	}
}