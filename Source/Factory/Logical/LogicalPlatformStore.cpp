#include "LogicalPlatformStore.h"

#include "Factory/Logical/LogicalShop.h"

ULogicalPlatformStore::ULogicalPlatformStore()
{
	;
}

void ULogicalPlatformStore::ShopTick(EGamePhase Phase)
{
	Super::ShopTick(Phase);

	if (ShopParent)
	{
		ShopParent->AddGold(1);
	}
}