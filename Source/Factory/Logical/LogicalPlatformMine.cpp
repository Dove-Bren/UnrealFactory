#include "LogicalPlatformMine.h"
#include "Engine/StaticMesh.h"

#include "Factory/Logical/LogicalShop.h"

#include <stdio.h>

#define WALL3_LENGTH 500
#define FLOOR_LENGTH 100

ULogicalPlatformMine::ULogicalPlatformMine() : ULogicalPlatform()
{
	this->FloorWidth = MINE_FLOOR_WIDTH;
	this->FloorHeight = MINE_FLOOR_HEIGHT;

	bStaticFloor = true; // Static size floor
}

void ULogicalPlatformMine::ShopTick(EGamePhase Phase)
{
	Super::ShopTick(Phase);

	if (ShopParent)
	{
		ShopParent->AddOre(1);
	}
}