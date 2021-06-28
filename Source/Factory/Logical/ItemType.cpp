#include "Factory/Logical/ItemType.h"

#include "Factory/FactorySingletons.h"
#include "Factory/FactoryLocale.h"
#include "Factory/Character/PlayerCharacter.h"
#include "Factory/Character/FactoryPlayerController.h"

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
		UItemType *Type = NewObject<UItemType>(GetTransientPackage(), ItemTypeClass);
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

UItemAction *UItemTypeBP::GetUseAction(EGamePlatform Platform, APlayerCharacter *Character, UItem *Item)
{
	//GetTransientPackage() ? donotcheckin TODO
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

	return NewObject<UItemAction>(this, ActionClass);
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