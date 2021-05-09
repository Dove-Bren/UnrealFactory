#include "LogicalPlatform.h"

#include "Factory/Logical/LogicalShop.h"

#define WALL3_LENGTH 500
#define FLOOR_LENGTH 100

#define WORLD_TO_CELL(X) ((int32) X / CELL_SIZE)

static inline FGridPosition MakeLocalInternal(ULogicalShop *ShopParent, float WorldX, float WorldY)
{
	FGridPosition Position = {};

	// Convert from the world position given to the grid position using our shop's XY as origin
	float OriginX = 0;
	float OriginY = 0;
	if (ShopParent)
	{
		OriginX = ShopParent->GetX();
		OriginY = ShopParent->GetY();
	}

	return Position;
}

#define MakeLocal(WorldX, WorldY) MakeLocalInternal(this->ShopParent, WorldX, WorldY)

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
	AddComponent(MakeLocal(WorldX, WorldY), Component);
}

void ULogicalPlatform::AddComponent(FGridPosition Position, ULogicalPlatformComponent *Component)
{
	ULogicalPlatformComponent *Existing = this->Grid->Insert(Position.X, Position.Y, Component, true);
	if (Existing)
	{
		Existing->RemoveFromPlatform(this);
	}
	Component->RegisterPlatform(this, Position);
}

FLocalLayout ULogicalPlatform::GetComponent(FGridPosition Position)
{
	FLocalLayout Layout = {};
	FGridPosition Orig = Position;

	Layout.Center = this->Grid->Get(Position.X, Position.Y);
	Position.Move(EDirection::NORTH);
	Layout.North = this->Grid->Get(Position.X, Position.Y);
	Position = Orig.Offset(EDirection::SOUTH);
	Layout.South = this->Grid->Get(Position.X, Position.Y);
	Position = Orig.Offset(EDirection::EAST);
	Layout.East = this->Grid->Get(Position.X, Position.Y);
	Position = Orig.Offset(EDirection::WEST);
	Layout.West = this->Grid->Get(Position.X, Position.Y);

	return Layout;
}

FLocalLayout ULogicalPlatform::GetComponentAt(float WorldX, float WorldY)
{
	return GetComponent(MakeLocal(WorldX, WorldY));

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