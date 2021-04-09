#pragma once

// 

#include "CoreMinimal.h"

#include "GameEnums.h"
#include "Platform.h"

#include "Shop.generated.h"

UCLASS(Blueprintable)
class UShop: public UObject
{
	GENERATED_BODY()
private:

	// Name of the shop
	UPROPERTY(VisibleAnywhere)
	FName Name;

protected:

	UShop() : UShop(FName(TEXT("Unnamed Shop"))) {};

	// Platforms that make up the shop
	UPROPERTY(VisibleAnywhere)
	TMap<EGamePlatform, UPlatform*> Platforms;

public:

	UShop(FName Name);

	const TMap<EGamePlatform, UPlatform*> GetPlatforms() { return this->Platforms; }
	const FName GetName() { return this->Name; }

	// Transition to the provided state, including stopping any activities from the previous one.
	// This is echoed down to all platforms in the shop.
	UFUNCTION(BlueprintCallable)
	void StartPhase(EGamePhase Phase);
};