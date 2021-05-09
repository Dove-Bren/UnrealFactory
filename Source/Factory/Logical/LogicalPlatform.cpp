#include "LogicalPlatform.h"

#include "Factory/Logical/LogicalShop.h"

#define WALL3_LENGTH 500
#define FLOOR_LENGTH 100

#define WORLD_TO_CELL(X) ((int32) X / CELL_SIZE)

ULogicalPlatform::ULogicalPlatform()
{
	Grid = CreateDefaultSubobject<UComponentLayout>(TEXT("Grid"));

	this->FloorWidth = MIN_PLATFORM_WIDTH * 2;
	this->FloorHeight = MIN_PLATFORM_HEIGHT * 2;
}

void ULogicalPlatform::AttachToShop(EGamePlatform Type, ULogicalShop *Shop)
{
	this->PlatformType = Type;
	this->ShopParent = Shop;
}

void ULogicalPlatform::AddComponentAt(float WorldX, float WorldY, ULogicalPlatformComponent *Component)
{
	FGridPosition Position = {};

	// Convert from the world position given to the grid position using our shop's XY as origin
	float OriginX = 0;
	float OriginY = 0;
	if (this->ShopParent)
	{
		OriginX = ShopParent->GetX();
		OriginY = ShopParent->GetY();
	}

	Position.X = WORLD_TO_CELL(WorldX - OriginX);
	Position.Y = WORLD_TO_CELL(WorldY - OriginY);

	AddComponent(Position, Component);
}

void ULogicalPlatform::AddComponent(FGridPosition Position, ULogicalPlatformComponent *Component)
{
	ULogicalPlatformComponent *Existing = this->Grid->Insert(Position.X, Position.Y, Component, true);
	if (Existing)
	{
		Existing->RemoveFromPlatform(this);
	}
	Component->RegisterPlatform(this);
}

void ULogicalPlatform::StartPhase(EGamePhase Phase)
{
	for (ULogicalPlatformComponent *comp : Grid->GetComponents())
	{
		comp->StartPhase(Phase);
	}
}

void ULogicalPlatform::ShopTick(EGamePhase Phase)
{
	for (ULogicalPlatformComponent *comp : Grid->GetComponents())
	{
		comp->ShopTick(Phase);
	}
}

void ULogicalPlatform::Resize(float Width, float Height)
{
	if (bStaticFloor)
	{
		return;
	}

	this->FloorWidth = Width;
	this->FloorHeight = Height;

	if (this->FloorWidth < MIN_PLATFORM_WIDTH)
	{
		this->FloorWidth = MIN_PLATFORM_WIDTH;
	}
	if (this->FloorHeight < MIN_PLATFORM_HEIGHT)
	{
		this->FloorHeight = MIN_PLATFORM_HEIGHT;
	}

	// TODO if we have a display platform, forward event?
	//SpawnWalls(this->FloorWidth, this->FloorHeight);
}