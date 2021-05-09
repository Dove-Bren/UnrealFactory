#include "Shop.h"

#include "Misc/EnumRange.h"

#include "Platform/PlatformStore.h"
#include "Platform/PlatformFactory.h"
#include "Platform/PlatformMine.h"

AShop::AShop()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	this->RootComponent = Mesh;
}

void AShop::SetupLogicalShop(ULogicalShop *LogicalShopIn)
{
	LogicalShop = LogicalShopIn;

	if (LogicalShop)
	{
		// Take position from parent shop
		this->SetActorLocation(FVector(LogicalShopIn->GetX(), LogicalShopIn->GetY(), LogicalShopIn->GetZ()));

		const TMap<EGamePlatform, ULogicalPlatform*> &LogicalPlatforms = LogicalShop->GetPlatforms();

		// Construct platform map
		for (EGamePlatform PlatformType : TEnumRange<EGamePlatform>())
		{
			UPlatform *Platform = MakePlatform(PlatformType);
			Platform->SetRelativeLocation(FVector(0, 0, GetPlatformOffset(PlatformType)), false);
			Platform->AttachToShop(PlatformType, this);
			Platform->SetLogicalPlatform(LogicalPlatforms[PlatformType]);
			this->Platforms.Add(PlatformType, Platform);
		}
	}
}

/*static*/ AShop *AShop::MakeShop(UWorld *World, ULogicalShop *LogicalShop)
{
	//AShop *Shop = NewObject<AShop>(LogicalShop, AShop::StaticClass());
	FActorSpawnParameters Parameters{};
	Parameters.Name = FName(FString::Printf(TEXT("%s_Actor"), *LogicalShop->GetName().ToString()));
	Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AShop *Shop = World->SpawnActor<AShop>(Parameters);
	Shop->SetupLogicalShop(LogicalShop);
	return Shop;
}

UPlatform *AShop::MakePlatform(EGamePlatform PlatformType)
{
	FName PlatformName = GetPlatformName(PlatformType);
	switch (PlatformType)
	{
	case EGamePlatform::STORE:
		return NewObject<UPlatformStore>(this, UPlatformStore::StaticClass(), PlatformName);
	case EGamePlatform::FACTORY:
		return NewObject<UPlatformFactory>(this, UPlatformFactory::StaticClass(), PlatformName);
	case EGamePlatform::MINE:
		return NewObject<UPlatformMine>(this, UPlatformMine::StaticClass(), PlatformName);
	}

	return NewObject<UPlatform>(this, UPlatform::StaticClass(), PlatformName);
}

void AShop::SetPlatformVisibility(EGamePlatform VisiblePlatform)
{
	for (EGamePlatform PlatformType : TEnumRange<EGamePlatform>())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Yellow, TEXT("Setting visibility"));
		UPlatform *Platform = Platforms[PlatformType];
		Platform->SetVisibility(PlatformType == VisiblePlatform, true);
	}
}