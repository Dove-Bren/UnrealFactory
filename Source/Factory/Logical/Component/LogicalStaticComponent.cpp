#include "LogicalStaticComponent.h"

#include "Factory/Building/Platform/Platform.h"
#include "Factory/Building/Platform/Component/NonLogical/StaticComponent.h"
#include "Factory/GameEnums.h"

ULogicalStaticComponent::ULogicalStaticComponent()
{
	this->bRemoveable = true;
	this->bMoveable = true;
	this->bRotatable = true;
}

APlatformComponent *ULogicalStaticComponent::SpawnWorldComponentInternal(UPlatform *Platform)
{
	FVector SpawnLoc;
	MakeSpawnLocation(SpawnLoc);
	AStaticComponent *WorldComp = Platform->GetWorld()->SpawnActor<AStaticComponent>(SpawnLoc, GetRotationFromDirection(Direction), MakeSpawnParams());
	WorldComp->SetLogicalStaticComponent(this);
	WorldComp->RegisterPlatform(Platform);
	return WorldComp;
}

void ULogicalItemStaticComponent::SetItemType(UItemType *NewItemType)
{
	this->ItemType = NewItemType;
	this->RemoveItemType = NewItemType;
}
