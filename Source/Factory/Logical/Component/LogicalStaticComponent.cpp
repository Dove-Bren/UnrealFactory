#include "LogicalStaticComponent.h"

#include "Factory/Building/Platform/Platform.h"
#include "Factory/Building/Platform/Component/NonLogical/StaticComponent.h"
#include "Factory/GameEnums.h"

APlatformComponent *ULogicalStaticComponent::SpawnWorldComponent(UPlatform *Platform)
{
	FVector SpawnLoc;
	MakeSpawnLocation(SpawnLoc);
	AStaticComponent *WorldComp = Platform->GetWorld()->SpawnActor<AStaticComponent>(SpawnLoc, GetRotationFromDirection(Direction), MakeSpawnParams());
	WorldComp->SetLogicalComponent(this);
	WorldComp->RegisterPlatform(Platform);
	return WorldComp;
}