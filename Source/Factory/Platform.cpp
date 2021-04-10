#include "Platform.h"

#include "Shop.h"

UPlatform::UPlatform()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	Mesh->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
}

void UPlatform::AttachToShop(EGamePlatform Type, AShop *Shop)
{
	this->PlatformType = Type;
	this->ShopParent = Shop;

	this->AttachToComponent(Shop->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
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