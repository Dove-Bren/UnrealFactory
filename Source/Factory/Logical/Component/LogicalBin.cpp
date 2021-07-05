#include "LogicalBin.h"

#include "Factory/GameEnums.h"
#include "Factory/FactoryLocale.h"
#include "Factory/Logical/Item.h"
#include "Factory/Building/Platform/Platform.h"
#include "Factory/Building/Platform/Component/Bin.h"
#include "Factory/Character/FactoryPlayerController.h"


APlatformComponent *ULogicalBin::SpawnWorldComponentInternal(UPlatform *Platform)
{
	FVector SpawnLoc;
	MakeSpawnLocation(SpawnLoc);
	ABin *WorldComp = Platform->GetWorld()->SpawnActor<ABin>(SpawnLoc, GetRotationFromDirection(Direction), MakeSpawnParams());
	WorldComp->SetLogicalBin(this);
	WorldComp->RegisterPlatform(Platform);
	return WorldComp;
}

void ULogicalBin::RemoveFromPlatform(ULogicalPlatform *Platform)
{
	Super::RemoveFromPlatform(Platform);

	; // TODO: drop items?
}

bool ULogicalBin::CanAccept_Implementation(EDirection DirectionIn, const UItem *ItemIn)
{
	return this->ItemType == nullptr
		|| (ItemIn->GetType() == ItemType
			&& ItemIn->GetCount() + ItemCount <= GetMaxItemCount());
}

UItem *ULogicalBin::InsertItem_Implementation(EDirection DirectionIn, UItem *ItemIn)
{
	UItem *Leftover = ItemIn;
	if (CanAccept_Implementation(DirectionIn, ItemIn))
	{
		if (!ItemType)
		{
			ItemType = ItemIn->GetType();
			ItemCount = ItemIn->GetCount();
			Leftover = nullptr;
		}
		else
		{
			int Room = GetMaxItemCount() - ItemCount;
			if (Room > 0)
			{
				if (Room >= ItemIn->GetCount())
				{
					this->ItemCount += ItemIn->GetCount();
					Leftover = nullptr;
				}
				else
				{
					this->ItemCount += Room;
					ItemIn->Split(Room);
					// leave Leftover as ItemIn to be cloned later
				}
			}
			// else Leftover = ItemIn
		}
	}

	if (Leftover == ItemIn)
	{
		Leftover = ItemIn->Clone();
	}

	return Leftover;
}

void ULogicalBin::PeekItems_Implementation(TArray<UItem*> &ItemArray)
{
	if (!!ItemType)
	{
		int32 Count = this->ItemCount;
		while (Count > 0)
		{
			int32 Amt = FMath::Min(Count, ItemType->GetMaxStackSize());

			UItem *Item = UItem::MakeItemEx(GetWorld(), ItemType);
			Item->SetCount(Amt);
			ItemArray.Add(Item);

			Count -= Amt;
		}
	}
}

bool ULogicalBin::CanTake_Implementation(EDirection DirectionIn, const UItem *ItemDemandOpt)
{
	bool Success = true;

	if (!ItemType || ItemCount <= 0)
	{
		Success = false;
	}
	else if (ItemDemandOpt)
	{
		if (ItemType != ItemDemandOpt->GetType() || ItemCount < ItemDemandOpt->GetCount())
		{
			Success = false;
		}
	}

	return Success;
}

UItem *ULogicalBin::TakeItem_Implementation(EDirection DirectionIn, const UItem *ItemDemandOpt)
{
	UItem *Taken = nullptr;

	if (!!ItemType && ItemCount > 0)
	{
		int32 Amt = -1;
		if (ItemDemandOpt)
		{
			if (ItemType == ItemDemandOpt->GetType())
			{
				Amt = FMath::Min(ItemCount, ItemDemandOpt->GetCount());
			}
		}
		else
		{
			Amt = FMath::Min(ItemCount, ItemType->GetMaxStackSize());
		}

		if (Amt > 0)
		{
			Taken = UItem::MakeItemEx(GetWorld(), ItemType);
			Taken->SetCount(Amt);

			this->ItemCount -= Amt;
			if (ItemCount <= 0)
			{
				ItemType = nullptr;
			}
		}
		
	}

	return Taken;
}

EDataValidationResult ULogicalBin::IsDataValid(TArray<FText> & ValidationErrors)
{
	EDataValidationResult result = ULogicalPlatformComponent::IsDataValid(ValidationErrors);
	bool bSuccess = true;

	if (this->MaxStackCount <= 0)
	{
		ValidationErrors.Add(LOCTEXT("LogicalBinFail.MaxStackMissing", "MaxStackCount must be greater than 0"));
		bSuccess = false;
	}
	if (!this->BinMesh)
	{
		ValidationErrors.Add(LOCTEXT("LogicalBinFail.BinMeshMissing", "A Mesh for the bin mut be specified"));
		bSuccess = false;
	}

	if (!bSuccess && result != EDataValidationResult::Invalid)
	{
		result = EDataValidationResult::Invalid;
	}

	return result;
}