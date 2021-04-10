#include "Shop.h"

#include "Misc/EnumRange.h"

AShop::AShop(FName Name)
{
	this->Name = Name;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	this->RootComponent = Mesh;

	// Construct platform map
	for (EGamePlatform PlatformType : TEnumRange<EGamePlatform>())
	{
		UPlatform *Platform = CreateDefaultSubobject<UPlatform>(GetPlatformName(PlatformType));
		Platform->AttachToShop(PlatformType, this);
		this->Platforms.Add(PlatformType, Platform);
	}

}

void AShop::StartPhase(EGamePhase Phase)
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

void AShop::ShopTick(EGamePhase Phase)
{
	for (EGamePlatform PlatformType : TEnumRange<EGamePlatform>())
	{
		UPlatform *Platform = (UPlatform*)(this->Platforms.Find(PlatformType));
		if (Platform)
		{
			Platform->ShopTick(Phase);
		}
	}
}

#define CLAMP_UNSIGNED(Amt, Count) uint32 Raw = Amt + Count;\
	if (Count < 0 && Raw > Resources.Gold)\
	{\
		/* Underflow */\
		Amt = 0;\
	}\
	else if (Count > 0 && Raw < Resources.Gold)\
	{\
		/* Overflow */\
		Amt = (uint32)(-1);\
	}\
	else\
	{\
		Amt = Raw;\
	}

void AShop::AddGold(int32 Count)
{
	CLAMP_UNSIGNED(Resources.Gold, Count);
}

void AShop::AddWood(int32 Count)
{
	CLAMP_UNSIGNED(Resources.Wood, Count);
}

void AShop::AddOre(int32 Count)
{
	CLAMP_UNSIGNED(Resources.Ore, Count);
}