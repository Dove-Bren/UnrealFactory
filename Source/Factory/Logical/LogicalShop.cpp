#include "LogicalShop.h"

#include "Misc/EnumRange.h"

#include "LogicalPlatformStore.h"
#include "LogicalPlatformFactory.h"
#include "LogicalPlatformMine.h"

ULogicalShop::ULogicalShop()
{

}
	
void ULogicalShop::InitShop(FName ShopName, float X, float Y, float Z)
{
	this->Name = ShopName;
	this->PosX = X;
	this->PosY = Y;
	this->PosZ = Z;

	// Construct platform map
	for (EGamePlatform PlatformType : TEnumRange<EGamePlatform>())
	{
		ULogicalPlatform *Platform = MakePlatform(PlatformType);
		Platform->AttachToShop(PlatformType, this);
		this->Platforms.Add(PlatformType, Platform);
	}

}

/*static*/ ULogicalShop *ULogicalShop::MakeShop(UWorld *World, FName Name, float X, float Y, float Z)
{
	ULogicalShop *Shop = NewObject<ULogicalShop>(World);
	Shop->InitShop(Name, X, Y, Z);
	return Shop;
}

ULogicalPlatform *ULogicalShop::MakePlatform(EGamePlatform PlatformType)
{
	FName PlatformName = GetPlatformName(PlatformType);
	switch (PlatformType)
	{
	case EGamePlatform::STORE:
		return NewObject<ULogicalPlatformStore>(this, ULogicalPlatformStore::StaticClass(), PlatformName);
	case EGamePlatform::FACTORY:
		return NewObject<ULogicalPlatformFactory>(this, ULogicalPlatformFactory::StaticClass(), PlatformName);
	case EGamePlatform::MINE:
		return NewObject<ULogicalPlatformMine>(this, ULogicalPlatformMine::StaticClass(), PlatformName);
	}

	return NewObject<ULogicalPlatform>(this, ULogicalPlatform::StaticClass(), PlatformName);
}

void ULogicalShop::StartPhase(EGamePhase Phase)
{
	for (EGamePlatform PlatformType : TEnumRange<EGamePlatform>())
	{
		ULogicalPlatform **Platform = this->Platforms.Find(PlatformType);
		if (Platform && *Platform)
		{
			(*Platform)->StartPhase(Phase);
		}
	}
}

void ULogicalShop::ShopTick(EGamePhase Phase)
{
	for (EGamePlatform PlatformType : TEnumRange<EGamePlatform>())
	{
		ULogicalPlatform **Platform = this->Platforms.Find(PlatformType);
		if (Platform && *Platform)
		{
			(*Platform)->ShopTick(Phase);
		}
	}
}

// Add Count to Amt, clamping to 0 and MAX_INT instead of wrapping
#define CLAMP_SIGNED(Amt, Count) int32 Raw = Amt + Count;\
	if (Count < 0 && Raw < 0)\
	{\
		/* Underflow */\
		Amt = 0;\
	}\
	else if (Count > 0 && Raw < 0)\
	{\
		/* Overflow */\
		Amt = MAX_int32;\
	}\
	else\
	{\
		Amt = Raw;\
	}

void ULogicalShop::AddGold(int32 Count)
{
	CLAMP_SIGNED(Resources.Gold, Count); 
}

void ULogicalShop::AddWood(int32 Count)
{
	CLAMP_SIGNED(Resources.Wood, Count);
}

void ULogicalShop::AddOre(int32 Count)
{
	CLAMP_SIGNED(Resources.Ore, Count);
}

void ULogicalShop::AddResource(EResourceType Type, int32 Count)
{
	switch (Type)
	{
	case EResourceType::GOLD:
	default:
		AddGold(Count);
		break;
	case EResourceType::WOOD:
		AddWood(Count);
		break;
	case EResourceType::ORE:
		AddOre(Count);
		break;
	}
}