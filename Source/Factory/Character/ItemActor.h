#pragma once

#include "CoreMinimal.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"

#include "Factory/Logical/Inventory/Item.h"

#include "ItemActor.generated.h"

// Represents an item that's not in a machine and instead is floating in the world.

UCLASS(BlueprintType)
class AItemActor : public AActor
{
	GENERATED_BODY()

public:

	void Setup(UItem *Item);

	virtual void Tick(float DeltaSeconds) override;

	UItem *GetItem() { return Item; }

	void SetItem(UItem *ItemIn) { this->Item = ItemIn; }

	// Create and spawn an ItemActor in the world at the given position.
	UFUNCTION(BlueprintCallable)
	static AItemActor *SpawnItemActor(UWorld *World, UItem *Item, FVector Location);

protected:

	AItemActor();
	virtual ~AItemActor() = default;

	// Main mesh component.
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *Mesh;

	// Movement component
	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent *MovementComponent;

	// Item this actor represents
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	UItem *Item;

private:

	int32 SpawnTicks;

	UFUNCTION()
	void OnActorOverlap(AActor *Actor, AActor *OtherActor);
};