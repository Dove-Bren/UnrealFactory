#include "Shop.h"

#include "Misc/EnumRange.h"

UShop::UShop(FName Name)
{
	this->Name = Name;

	// Construct platform map
	for (EGamePlatform PlatformType : TEnumRange<EGamePlatform>())
	{
		UPlatform *Platform = CreateDefaultSubobject<UPlatform>(GetPlatformName(PlatformType));
		Platform->AttachToShop(PlatformType, this);
		this->Platforms.Add(PlatformType, Platform);
	}

}

void UShop::StartPhase(EGamePhase Phase)
{
	for (EGamePlatform PlatformType : TEnumRange<EGamePlatform>())
	{
		UPlatform *Platform = (UPlatform*)(this->Platforms.Find(PlatformType));
		if (Platform)
		{
			Platform->StartPhase(Phase);
		}
	}
}