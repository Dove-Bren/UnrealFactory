#include "Platform.h"

#include "Shop.h"

#define CELL_SIZE 100

#define WORLD_TO_CELL(X) ((int32) X / CELL_SIZE)

UPlatform::UPlatform()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	Mesh->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	Grid = CreateDefaultSubobject<UComponentLayout>(TEXT("Grid"));
}

void UPlatform::AttachToShop(EGamePlatform Type, AShop *Shop)
{
	this->PlatformType = Type;
	this->ShopParent = Shop;

	this->AttachToComponent(Shop->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}

void UPlatform::AddComponent(APlatformComponent *Component)
{
	// Calculate where on the grid the component should go
	int32 CellX = WORLD_TO_CELL(Component->GetActorLocation().X);
	int32 CellY = WORLD_TO_CELL(Component->GetActorLocation().Y);

	APlatformComponent *Existing = this->Grid->Insert(CellX, CellY, Component, true);
	if (Existing)
	{
		Existing->RemoveFromPlatform(this);
	}
	Component->RegisterPlatform(this);
}

void UPlatform::StartPhase(EGamePhase Phase)
{
	for (APlatformComponent *comp : Grid->GetComponents())
	{
		comp->StartPhase(Phase);
	}
}

void UPlatform::ShopTick(EGamePhase Phase)
{
	for (APlatformComponent *comp : Grid->GetComponents())
	{
		comp->ShopTick(Phase);
	}
}