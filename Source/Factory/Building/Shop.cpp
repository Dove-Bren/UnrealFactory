#include "Shop.h"

#include "Misc/EnumRange.h"

#include "Platform/PlatformStore.h"
#include "Platform/PlatformFactory.h"
#include "Platform/PlatformMine.h"

AShop::AShop()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	this->RootComponent = Mesh;

	this->PrimaryActorTick.bCanEverTick = true;
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
		UPlatform *Platform = Platforms[PlatformType];
		Platform->SetVisibility(PlatformType == VisiblePlatform, true);
	}
}

void AShop::TickShop(EGamePhase Phase)
{
	for (EGamePlatform PlatformType : TEnumRange<EGamePlatform>())
	{
		UPlatform *Platform = Platforms[PlatformType];
		Platform->ShopTick(Phase);
	}
}

// TODO actually maybe just unify this with regular factory ticking?
// Or maybe this will have a faster tick rate since it's the visual world representation?
void AShop::Tick(float DeltaSeconds)
{
	static float SecondBuildup = 0;
	SecondBuildup += DeltaSeconds;

	Super::Tick(DeltaSeconds);

	// Shouldn't be here. Shop registry?
#define TICKS_PER_SECOND 60.0f
#define SECONDS_PER_TICK (1.0f / TICKS_PER_SECOND)

	while (SecondBuildup >= SECONDS_PER_TICK)
	{
		SecondBuildup -= SECONDS_PER_TICK;
		TickShop(EGamePhase::MORNING); // TODO
	}
#undef TICKS_PER_SECOND
#undef SECONDS_PER_TICK
}