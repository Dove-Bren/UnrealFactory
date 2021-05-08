#pragma once

// 

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"

#include "Factory/GameEnums.h"
#include "Platform/Platform.h"
#include "ShopResources.h"

#include "Shop.generated.h"

UCLASS(Blueprintable)
class AShop: public AActor
{
	GENERATED_BODY()
private:

	// Name of the shop
	UPROPERTY(VisibleAnywhere)
	FName Name;

	// Main mesh component
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *Mesh;

	// Resources available to shop
	UPROPERTY(VisibleAnywhere)
	FShopResources Resources;

	// TODO keep history of resources for graphing? Maybe capture every phase change?


protected:

	AShop() : AShop(FName(TEXT("Unnamed Shop"))) {};

	// Create the platform for the provided platform type.
	virtual UPlatform *MakePlatform(EGamePlatform PlatformType);

	// Platforms that make up the shop
	UPROPERTY(VisibleAnywhere)
	TMap<EGamePlatform, UPlatform*> Platforms;

public:

	AShop(FName Name);

	const TMap<EGamePlatform, UPlatform*> & GetPlatforms() const { return Platforms; }
	const FName & GetName() const { return this->Name; }
	const FShopResources &GetResources() const { return this->Resources; }

	// Add some gold to the shop
	UFUNCTION(BlueprintCallable)
	void AddGold(int32 Count);

	// Add some available wood to the shop
	UFUNCTION(BlueprintCallable)
	void AddWood(int32 Count);

	// Add some avaialble ore to the shop
	UFUNCTION(BlueprintCallable)
	void AddOre(int32 Count);

	// Transition to the provided state, including stopping any activities from the previous one.
	// This is echoed down to all platforms in the shop.
	UFUNCTION(BlueprintCallable)
	void StartPhase(EGamePhase Phase);

	// Tick this shop and all platforms and components
	UFUNCTION(BlueprintCallable)
	void ShopTick(EGamePhase Phase);
};