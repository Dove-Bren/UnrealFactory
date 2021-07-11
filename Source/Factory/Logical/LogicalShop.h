#pragma once

// 

#include "CoreMinimal.h"

#include "Factory/GameEnums.h"
#include "LogicalPlatform.h"
#include "ShopResources.h"

#include "LogicalShop.generated.h"

UCLASS()
class ULogicalShop: public UObject
{
	GENERATED_BODY()
private:

	// TODO keep history of resources for graphing? Maybe capture every phase change?

	// Center X world position of the shop
	UPROPERTY(VisibleAnywhere)
	float PosX;

	// Center Y world position of the shop
	UPROPERTY(VisibleAnywhere)
	float PosY;

	// Bottom Z world position of the shop
	UPROPERTY(VisibleAnywhere)
	float PosZ;


protected:

	ULogicalShop();
	void InitShop(FName Name, float X, float Y, float Z);

	// Create the platform for the provided platform type.
	virtual ULogicalPlatform *MakePlatform(EGamePlatform PlatformType);

	// Platforms that make up the shop
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<EGamePlatform, ULogicalPlatform*> Platforms;

	// Name of the shop
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName Name;

	// Resources available to shop
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FShopResources Resources {};

public:

	static ULogicalShop *MakeShop(UWorld *World, FName ShopName, float X, float Y, float Z);

	const TMap<EGamePlatform, ULogicalPlatform*> & GetPlatforms() const { return Platforms; }
	const FName & GetName() const { return this->Name; }
	float GetX() const { return PosX; }
	float GetY() const { return PosY; }
	float GetZ() const { return PosZ; }
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

	int32 GetResource(EResourceType Type) { return Resources.GetCount(Type); }
	void AddResource(EResourceType Type, int32 Count);

	// Transition to the provided state, including stopping any activities from the previous one.
	// This is echoed down to all platforms in the shop.
	UFUNCTION(BlueprintCallable)
	virtual void StartPhase(EGamePhase Phase);

	// Tick this shop and all platforms and components
	UFUNCTION(BlueprintCallable)
	virtual void ShopTick(EGamePhase Phase);
};