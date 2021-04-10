#pragma once

// 

#include "CoreMinimal.h"

#include "GameEnums.h"
#include "Platform.h"
#include "ShopResources.h"

#include "Shop.generated.h"

UCLASS(Blueprintable)
class UShop: public UObject
{
	GENERATED_BODY()
private:

	// Name of the shop
	UPROPERTY(VisibleAnywhere)
	FName Name;

	// Resources available to shop
	UPROPERTY(EditAnywhere)
	FShopResources Resources;

	// TODO keep history of resources for graphing? Maybe capture every phase change?


protected:

	UShop() : UShop(FName(TEXT("Unnamed Shop"))) {};

	// Platforms that make up the shop
	UPROPERTY(VisibleAnywhere)
	TMap<EGamePlatform, UPlatform*> Platforms;

public:

	UShop(FName Name);

	const TMap<EGamePlatform, UPlatform*> GetPlatforms() { return this->Platforms; }
	const FName GetName() { return this->Name; }
	const FShopResources &GetResources() { return this->Resources; }

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