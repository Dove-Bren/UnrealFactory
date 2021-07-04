#pragma once

// 

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

#include "Factory/GameEnums.h"
#include "Factory/Logical/LogicalShop.h"
#include "Factory/Logical/LogicalPlatform.h"
#include "Platform/Platform.h"

#include "Shop.generated.h"

UCLASS(Blueprintable)
class AShop: public AActor
{
	GENERATED_BODY()
private:

	// Link to logical shop object
	UPROPERTY(VisibleAnywhere)
	ULogicalShop *LogicalShop;

	// Main mesh component
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *Mesh;

protected:

	AShop();
	virtual ~AShop() = default;

	// Create the platform for the provided platform type.
	virtual UPlatform *MakePlatform(EGamePlatform PlatformType);

	// Physical platforms that make up the shop
	UPROPERTY(VisibleAnywhere)
	TMap<EGamePlatform, UPlatform*> Platforms;

	void SetupLogicalShop(ULogicalShop *LogicalShop);

public:

	static AShop *MakeShop(UWorld *World, ULogicalShop *LogicalShop);

	const TMap<EGamePlatform, UPlatform*> & GetPlatforms() const { return Platforms; }

	// Get the logical shop object associated with this world shop, if any.
	UFUNCTION(BlueprintCallable)
	ULogicalShop *GetLogicalShop() const { return LogicalShop; }

	// Sets only the provided platform type to be visible
	UFUNCTION(BlueprintCallable)
	void SetPlatformVisibility(EGamePlatform VisiblePlatform);
};