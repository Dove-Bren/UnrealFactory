#pragma once

// Sort of 'dummy' platform component that just takes up space

#include "CoreMinimal.h"

#include "Engine/StaticMesh.h"

#include "LogicalPlatformComponent.h"
#include "Factory/Logical/Inventory/ItemType.h"
#include "Factory/Logical/Inventory/Item.h"

#include "LogicalStaticComponent.generated.h"

UCLASS(BlueprintType)
class ULogicalStaticComponent : public ULogicalPlatformComponent
{
	GENERATED_BODY()

private:


public:

	ULogicalStaticComponent();
	virtual ~ULogicalStaticComponent() = default;

	virtual UStaticMesh *GetStaticMesh() { return nullptr; };

	virtual APlatformComponent *SpawnWorldComponentInternal(UPlatform *Platform) override;
};

UCLASS(BlueprintType)
class ULogicalItemStaticComponent : public ULogicalStaticComponent
{
	GENERATED_BODY()

private:

protected:

	UPROPERTY(VisibleInstanceOnly)
	UItemType *ItemType;

public:

	ULogicalItemStaticComponent() : ULogicalStaticComponent() {};
	virtual ~ULogicalItemStaticComponent() = default;

	// Set the type of item this static component was made from.
	// Used to pick the item back up and derive actions from.
	UFUNCTION(BlueprintCallable)
	void SetItemType(UItemType *NewItemType);

	// Get the item that this static component was made from.
	UFUNCTION(BlueprintCallable)
	UItemType *GetItemType() const { return this->ItemType; }

	virtual UStaticMesh *GetStaticMesh() override { return ItemType ? ItemType->GetMesh() : nullptr; }

	virtual void OnInitialPlacement(APlayerCharacter *PlacingCharacter, UItem *Item, EDirection DirectionIn) override
	{
		ULogicalStaticComponent::OnInitialPlacement(PlacingCharacter, Item, DirectionIn);
		if (Item) SetItemType(Item->GetType()); 
	}
};