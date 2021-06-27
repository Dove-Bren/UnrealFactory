#include "FactorySingletons.h"

#include "Engine/World.h"

#include "Kismet/GameplayStatics.h"

/*static*/ UFactorySingletons *UFactorySingletons::GetInstance(const UObject *WorldContextObject)
{
	UWorld *World = WorldContextObject->GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("World is empty!"));
	}
	return Cast<UFactorySingletons>(World->GetGameInstance());
}

UFactorySingletons::UFactorySingletons()
{
	ItemRegistry = CreateDefaultSubobject<UItemTypeRegistry>(TEXT("ItemRegistry"));
}