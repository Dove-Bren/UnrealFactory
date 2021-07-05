#pragma once

// Sort of 'dummy' platform component that just takes up space

#include "CoreMinimal.h"

#include "Engine/StaticMesh.h"

#include "LogicalPlatformComponent.h"
#include "Factory/Logical/ItemType.h"
#include "Factory/Logical/Item.h"

#include "LogicalStaticComponent.generated.h"

UCLASS(BlueprintType)
class ULogicalStaticComponent : public ULogicalPlatformComponent
{
	GENERATED_BODY()

private:


public:

	ULogicalStaticComponent() {};
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

	UItemType *ItemType;

public:

	ULogicalItemStaticComponent() : ULogicalStaticComponent() {};
	virtual ~ULogicalItemStaticComponent() = default;

	void SetItemType(UItemType *NewItemType) { this->ItemType = NewItemType; }

	UItemType *GetItemType() const { return this->ItemType; }

	virtual UStaticMesh *GetStaticMesh() override { return ItemType ? ItemType->GetMesh() : nullptr; }

	virtual void OnInitialPlacement(APlayerCharacter *PlacingCharacter, UItem *Item, EDirection DirectionIn) override
	{
		ULogicalStaticComponent::OnInitialPlacement(PlacingCharacter, Item, DirectionIn);
		if (Item) SetItemType(Item->GetType()); 
	}
};