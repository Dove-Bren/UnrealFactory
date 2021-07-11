#include "LogicalResourceChute.h"

#include "Factory/GameEnums.h"
#include "Factory/FactoryLocale.h"
#include "Factory/Logical/Item.h"
#include "Factory/Logical/LogicalPlatform.h"
#include "Factory/Logical/LogicalShop.h"
#include "Factory/Building/Platform/Platform.h"
#include "Factory/Building/Platform/Component/Chute.h"

APlatformComponent *ULogicalResourceChute::SpawnWorldComponentInternal(UPlatform *Platform)
{
	FVector SpawnLoc;
	MakeSpawnLocation(SpawnLoc);
	AChute *WorldComp = Platform->GetWorld()->SpawnActor<AChute>(SpawnLoc, GetRotationFromDirection(Direction), MakeSpawnParams());
	WorldComp->SetLogicalChute(this);
	WorldComp->RegisterPlatform(Platform);
	return WorldComp;
}

ULogicalShop *ULogicalResourceChute::GetShop()
{
	ULogicalPlatform *Platform = GetParentPlatform();
	if (Platform)
	{
		return Platform->GetShop();
	}
	else
	{
		return nullptr;
	}
}

bool ULogicalResourceChute::CanAccept_Implementation(EDirection DirectionIn, const UItem *ItemIn)
{
	return false;
}

UItem *ULogicalResourceChute::InsertItem_Implementation(EDirection DirectionIn, UItem *ItemIn)
{
	return ItemIn;
}

void ULogicalResourceChute::PeekItems_Implementation(TArray<UItem*> &ItemArray)
{
	if (!!GetShop() && GetShop()->GetResources().GetCount(this->ResourceType) > 0)
	{
		UItem *Item = UItem::MakeItem(GetWorld(), ItemType);
		Item->SetCount(1);
		ItemArray.Add(Item);
	}
}

bool ULogicalResourceChute::CanTake_Implementation(EDirection DirectionIn, const UItem *ItemDemandOpt)
{
	bool Success = false;

	if (!!GetShop() && GetShop()->GetResources().GetCount(this->ResourceType) > 0)
	{
		if (ItemDemandOpt)
		{
			if (UItemTypeRegistry::GetInstance(GetWorld())->GetDefaultInstance(ItemType) != ItemDemandOpt->GetType()
				|| GetShop()->GetResources().GetCount(this->ResourceType) < ItemDemandOpt->GetCount())
			{
				Success = false;
			}
		}
		// else Success = true
	}

	return Success;
}

UItem *ULogicalResourceChute::TakeItem_Implementation(EDirection DirectionIn, const UItem *ItemDemandOpt)
{
	UItem *Taken = nullptr;
	int32 ResourceCount = GetShop()->GetResources().GetCount(this->ResourceType);

	if (!!GetShop() && ResourceCount > 0 && (!ItemDemandOpt || ItemDemandOpt->GetType() == UItemTypeRegistry::GetInstance(GetWorld())->GetDefaultInstance(ItemType)))
	{
		int Count = FMath::Min(ResourceCount, ItemDemandOpt ? ItemDemandOpt->GetCount() : 1);
		Taken = UItem::MakeItem(GetWorld(), ItemType);
		Taken->SetCount(Count);
		GetShop()->AddResource(ResourceType, -Count);
	}

	return Taken;
}

EDataValidationResult ULogicalResourceChute::IsDataValid(TArray<FText> & ValidationErrors)
{
	EDataValidationResult result = ULogicalPlatformComponent::IsDataValid(ValidationErrors);
	bool bSuccess = true;

	if (!this->ItemType)
	{
		ValidationErrors.Add(LOCTEXT("LogicalChuteFail.MaxStackMissing", "An ItemType must be specified"));
		bSuccess = false;
	}
	if (!this->ChuteMesh)
	{
		ValidationErrors.Add(LOCTEXT("LogicalChuteFail.ChuteMeshMissing", "A Mesh for the chute mut be specified"));
		bSuccess = false;
	}

	if (!bSuccess && result != EDataValidationResult::Invalid)
	{
		result = EDataValidationResult::Invalid;
	}

	return result;
}

void ULogicalResourceChute::ShopTick(EGamePhase Phase)
{
	Super::ShopTick(Phase);

	if (!GetShop())
	{
		return;
	}

	int32 ResourceCount = GetShop()->GetResources().GetCount(this->ResourceType);

	if (ResourceCount > 0)
	{
		FLocalLayout Layout;
		FixupLocalLayout(Layout);

		UItem *Sample = nullptr;

		for (EDirection Dir : TEnumRange<EDirection>())
		{
			ULogicalPlatformComponent *Comp = Layout.GetDirection(Dir);
			if (Comp && Comp->Implements<UItemHandler>())
			{
				IItemHandler *Handler = Cast<IItemHandler>(Comp);
				check(!!Handler);

				if (!Sample)
				{
					Sample = UItem::MakeItem(GetWorld(), ItemType);
					Sample->SetCount(1);
				}

				if (nullptr == IItemHandler::AttemptInsert(Handler, OppositeDirection(Dir), Sample->Clone()))
				{
					GetShop()->AddResource(ResourceType, -1);
					ResourceCount = GetShop()->GetResources().GetCount(this->ResourceType);
					if (ResourceCount <= 0)
					{
						break;
					}
				}
			}
		}
	}
}
