#pragma once

// 

#include "CoreMinimal.h"

#include "Factory/GameEnums.h"

#include "Engine/StaticMesh.h"
#include "Engine/Texture2D.h"

#include "ItemType.generated.h"

typedef class APlayerCharacter;
typedef class UItem;
typedef class UItemAction;
typedef class ULogicalPlatform;
typedef class ULogicalPlatformComponent;
typedef class IInventoryBase;

UCLASS(BlueprintType, Abstract)
class UItemType : public UObject
{
	GENERATED_BODY()
protected:

	friend class UItemTypeRegistry;

	// Display name of the item type
	UPROPERTY(EditAnywhere)
	FName Name;

	// Description of the item type
	UPROPERTY(EditAnywhere)
	FName Description;

	// Internal ID of the item
	UPROPERTY(EditAnywhere)
	FName ID;

	// Max number of these items that can stack in one slot/Item object
	UPROPERTY(EditAnywhere)
	int32 MaxStackSize;

	// Visual mesh for the item
	UPROPERTY(EditAnywhere)
	UStaticMesh *DisplayMesh;

	// Visual mesh for the item
	UPROPERTY(EditAnywhere)
	UTexture2D *DisplaySprite;

public:

	UItemType() {};
	virtual ~UItemType() = default;

	// Get the maximum count of this item type allowed in a single Item instance
	UFUNCTION(BlueprintCallable)
	virtual int32 GetMaxStackSize() const { return MaxStackSize; }

	// Get the display name of this item type
	UFUNCTION(BlueprintCallable)
	virtual FName GetName() const { return Name; }

	// Get the description of this item type
	UFUNCTION(BlueprintCallable)
	virtual FName GetDescription() const { return Description; }

	// Get the internal registry name for this item type
	UFUNCTION(BlueprintCallable)
	virtual FName GetRegistryName() const { return ID; }

	// Get the display mesh for this item type
	UFUNCTION(BlueprintCallable)
	virtual UStaticMesh *GetMesh() const { return DisplayMesh; }

	// Get the 2D sprite for displaying this item type in the inventory
	UFUNCTION(BlueprintCallable)
	virtual UTexture2D *GetSprite() const { return DisplaySprite; }

	// Get what action should be taken when an item is selected for use in an inventory
	UFUNCTION(BlueprintCallable)
	virtual UItemAction *GetUseAction(EGamePlatform Platform, APlayerCharacter *Character, UItem *Item) PURE_VIRTUAL(UItemType::GetUseAction, return nullptr;);

	virtual TSubclassOf<ULogicalPlatformComponent> GetPlatformComponentClass(EGamePlatform Platform, const UItem *Item) const;

	// Create and return the platform component to use when this item type is 'placed' on a platform
	// May return nullptr
	virtual ULogicalPlatformComponent *SpawnPlatformComponent(EGamePlatform Platform, ULogicalPlatform *LogicalPlatform, APlayerCharacter *Character, UItem *Item);
};

UCLASS(Blueprintable, Abstract)
class UItemTypeBP : public UItemType
{
	GENERATED_BODY()

protected:

	// Which ItemAction type to use on STORE layers
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UItemAction> StoreActionClass;

	// Which ItemAction type to use on FACTORY layers
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UItemAction> FactoryActionClass;

	// Which ItemAction type to use on MINE layers
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UItemAction> MineActionClass;

public:

	UItemTypeBP() : UItemType() {};
	virtual ~UItemTypeBP() = default;

	virtual UItemAction *GetUseAction(EGamePlatform Platform, APlayerCharacter *Character, UItem *Item) override;

	virtual EDataValidationResult IsDataValid(TArray<FText> & ValidationErrors) override;
};

UCLASS(Blueprintable, Abstract)
class UPlaceableItemTypeBP : public UItemTypeBP
{
	GENERATED_BODY()

protected:

	// Which Platform Component type to spawn on STORE layers
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ULogicalPlatformComponent> StoreComponentClass;

	// Which Platform Component type to spawn on FACTORY layers
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ULogicalPlatformComponent> FactoryComponentClass;

	// Which Platform Component type to spawn on MINE layers
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ULogicalPlatformComponent> MineComponentClass;

public:

	UPlaceableItemTypeBP() : UItemTypeBP() {};
	virtual ~UPlaceableItemTypeBP() = default;

	virtual TSubclassOf<ULogicalPlatformComponent> GetPlatformComponentClass(EGamePlatform Platform, const UItem *Item) const override;
	virtual ULogicalPlatformComponent *SpawnPlatformComponent(EGamePlatform Platform, ULogicalPlatform *LogicalPlatform, APlayerCharacter *Character, UItem *Item) override;
};

UCLASS(BlueprintType, NotBlueprintable)
class UItemTypeRegistry : public UObject
{
	GENERATED_BODY()

public:
	
	static UItemTypeRegistry *GetInstance(const UObject *WorldContextObject);

	UItemType *GetDefaultInstance(TSubclassOf<UItemType> ItemTypeClass);
	UItemType *GetType(FName Name);

	void Register(UItemType *Type);

protected:

	UItemTypeRegistry();
	~UItemTypeRegistry();

	UPROPERTY()
	TMap<FName, UItemType*> Registry;

	UPROPERTY()
	TMap<UClass*, UItemType*> ClassRegistry;
};

UCLASS(Abstract)
class UItemAction : public UObject
{
	GENERATED_BODY()
protected:
	EItemActionType ActionType;

	UItemAction();
public:

	UItemAction(EItemActionType Type);
	virtual ~UItemAction();

	// Return what type of action this is
	UFUNCTION(BlueprintCallable)
	virtual EItemActionType GetType() { return ActionType; }

	// Perform the action
	UFUNCTION(BlueprintCallable)
	virtual void Perform(APlayerCharacter *Character, TScriptInterface<IInventoryBase> Inventory, int32 SlotIdx, UItem *Item) PURE_VIRTUAL(UItemAction::Perform, );
};

UCLASS(Blueprintable, Abstract)
class UItemActionBP : public UItemAction
{
	GENERATED_BODY()
protected:

	UItemActionBP();
public:

	virtual ~UItemActionBP() = default;

	virtual void Perform(APlayerCharacter *Character, TScriptInterface<IInventoryBase> Inventory, int32 SlotIdx, UItem *Item) override;

	// ^ set up above to be implementable via blueprint. Idk if I can do any
	// sort of validation that it is but I guess that's okay for blueprints.

	// Blueprint-overrideable version of the Perform function
	UFUNCTION(BlueprintImplementableEvent)
	void PerformBP(APlayerCharacter *Character, TScriptInterface<IInventoryBase> &Inventory, int32 SlotIdx, UItem *Item);

	// Then make the four basic classes?
};

UCLASS(Blueprintable)
class UItemActionNoAction : public UItemAction
{
	GENERATED_BODY()
protected:

	UItemActionNoAction() : UItemAction(EItemActionType::NO_ACTION) {}
public:

	virtual ~UItemActionNoAction() = default;

	virtual void Perform(APlayerCharacter *Character, TScriptInterface<IInventoryBase> Inventory, int32 SlotIdx, UItem *Item) override {};
};

UCLASS(Blueprintable)
class UItemActionPlace : public UItemAction
{
	GENERATED_BODY()
protected:

	UItemActionPlace() : UItemAction(EItemActionType::PLACE) {}
public:

	virtual ~UItemActionPlace() = default;

	virtual void Perform(APlayerCharacter *Character, TScriptInterface<IInventoryBase> Inventory, int32 SlotIdx, UItem *Item) override;
};

UCLASS(Blueprintable)
class UItemActionEquip : public UItemAction
{
	GENERATED_BODY()
protected:

	UItemActionEquip() : UItemAction(EItemActionType::EQUIP) {}
public:

	virtual ~UItemActionEquip() = default;

	virtual void Perform(APlayerCharacter *Character, TScriptInterface<IInventoryBase> Inventory, int32 SlotIdx, UItem *Item) override;
};