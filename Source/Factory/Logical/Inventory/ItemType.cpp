#include "ItemType.h"

#include "Factory/FactorySingletons.h"
#include "Factory/FactoryLocale.h"
#include "Factory/Character/PlayerCharacter.h"
#include "Factory/Character/FactoryPlayerController.h"
#include "Factory/Logical/Component/LogicalStaticComponent.h"

/*static*/ UItemTypeRegistry *UItemTypeRegistry::GetInstance(const UObject *WorldContextObject)
{
	UFactorySingletons *Singletons = UFactorySingletons::GetInstance(WorldContextObject);
	return Singletons ? Singletons->GetItemRegistry() : nullptr;
}

UItemTypeRegistry::UItemTypeRegistry()
{
	;
}

UItemTypeRegistry::~UItemTypeRegistry()
{
	;
}

UItemType *UItemTypeRegistry::GetDefaultInstance(TSubclassOf<UItemType> ItemTypeClass)
{
	UItemType **Lookup = ClassRegistry.Find(ItemTypeClass);
	if (Lookup)
	{
		return *Lookup;
	}
	else
	{
		UItemType *Type = NewObject<UItemType>(this, ItemTypeClass);
		Register(Type);
		return Type;
	}

	return nullptr;
}

UItemType *UItemTypeRegistry::GetType(FName Name)
{
	UItemType **Lookup = Registry.Find(Name);
	if (Lookup)
	{
		return *Lookup;
	}

	return nullptr;
}

void UItemTypeRegistry::Register(UItemType *Type)
{
	if (Type->GetRegistryName().IsNone())
	{
		static int sFailIdx = 0;
		UE_LOG(LogTemp, Error, TEXT("ItemType did not have RegistryName/ID set! Setting to name or random"));
		if (!Type->GetName().IsNone())
		{
			Type->ID = Type->GetName();
		}
		else
		{
			Type->ID = *FString::Printf(TEXT("ITEMTYPE_GEN_%d"), sFailIdx++);
		}
	}

	Registry.Add(Type->GetRegistryName(), Type);
	ClassRegistry.Add(Type->GetClass(), Type);
}

TSubclassOf<ULogicalPlatformComponent> UItemType::GetPlatformComponentClass(EGamePlatform Platform, const UItem *Item) const
{
	return ULogicalItemStaticComponent::StaticClass();
}

ULogicalPlatformComponent *UItemType::SpawnPlatformComponent(EGamePlatform Platform, ULogicalPlatform *LogicalPlatform, APlayerCharacter *Character, UItem *Item)
{
	return NewObject<ULogicalItemStaticComponent>(LogicalPlatform);
}

UItemAction *UItemTypeBP::GetUseAction(EGamePlatform Platform, APlayerCharacter *Character, UItem *Item)
{
	TSubclassOf<UItemAction> ActionClass;
	switch (Platform)
	{
	case EGamePlatform::STORE:
	default:
		ActionClass = this->StoreActionClass;
		break;
	case EGamePlatform::FACTORY:
		ActionClass = this->FactoryActionClass;
		break;
	case EGamePlatform::MINE:
		ActionClass = this->MineActionClass;
		break;
	}

	return NewObject<UItemAction>(Character, ActionClass);
}

EDataValidationResult UItemTypeBP::IsDataValid(TArray<FText> & ValidationErrors)
{
	EDataValidationResult result = UItemType::IsDataValid(ValidationErrors);
	bool bSuccess = true;

	if (!this->StoreActionClass)
	{
		ValidationErrors.Add(LOCTEXT("ItemTypeFail.StoreActionMissing", "Store Action must be specified"));
		bSuccess = false;
	}
	if (!this->FactoryActionClass)
	{
		ValidationErrors.Add(LOCTEXT("ItemTypeFail.FactoryActionMissing", "Factory Action must be specified"));
		bSuccess = false;
	}
	if (!this->MineActionClass)
	{
		ValidationErrors.Add(LOCTEXT("ItemTypeFail.MineActionMissing", "Mine Action must be specified"));
		bSuccess = false;
	}

	if (!bSuccess && result != EDataValidationResult::Invalid)
	{
		result = EDataValidationResult::Invalid;
	}

	return result;
}

TSubclassOf<ULogicalPlatformComponent> UPlaceableItemTypeBP::GetPlatformComponentClass(EGamePlatform Platform, const UItem *Item) const
{
	TSubclassOf<ULogicalPlatformComponent> ComponentClass;
	switch (Platform)
	{
	case EGamePlatform::STORE:
	default:
		ComponentClass = this->StoreComponentClass;
		break;
	case EGamePlatform::FACTORY:
		ComponentClass = this->FactoryComponentClass;
		break;
	case EGamePlatform::MINE:
		ComponentClass = this->MineComponentClass;
		break;
	}
	return ComponentClass;
}

ULogicalPlatformComponent *UPlaceableItemTypeBP::SpawnPlatformComponent(EGamePlatform Platform, ULogicalPlatform *LogicalPlatform, APlayerCharacter *Character, UItem *Item)
{
	TSubclassOf<ULogicalPlatformComponent> ComponentClass = this->GetPlatformComponentClass(Platform, Item);
	return NewObject<ULogicalPlatformComponent>(LogicalPlatform, ComponentClass);
}

UItemAction::UItemAction() : UItemAction(EItemActionType::NO_ACTION)
{

}

UItemAction::UItemAction(EItemActionType Type) : ActionType(Type)
{

}

UItemAction::~UItemAction()
{

}

UItemActionBP::UItemActionBP()
{

}

void UItemActionBP::Perform(APlayerCharacter *Character, UInventory *Inventory, int32 SlotIdx, UItem *Item)
{
	this->PerformBP(Character, Inventory, SlotIdx, Item);
}

void UItemActionPlace::Perform(APlayerCharacter *Character, UInventory *Inventory, int32 SlotIdx, UItem *Item)
{
	AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(Character->GetController());
	if (Controller)
	{
		Controller->SetActiveMouseItem(UInventorySlotRef::MakeRef(Controller, Inventory, SlotIdx));
	}
}

void UItemActionEquip::Perform(APlayerCharacter *Character, UInventory *Inventory, int32 SlotIdx, UItem *Item)
{

}