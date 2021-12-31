#include "StaticComponent.h"

#include "Factory/Character/FactoryPlayerController.h"
#include "Factory/Character/PlayerCharacter.h"

void AStaticComponent::RefreshMesh()
{
	if (LogicalComponent)
	{
		UStaticMesh *MeshAsset = LogicalComponent->GetStaticMesh();
		if (MeshAsset)
		{
			this->Mesh->SetStaticMesh(MeshAsset);
			// Also take and set a material?
		}

		if (Cast<ULogicalItemStaticComponent>(LogicalComponent) != nullptr)
		{
			// Has an item
			this->bHighlight = true;
		}
		else
		{
			this->bHighlight = false;
		}
	}
	else
	{
		this->bHighlight = false;
	}
}

bool AStaticComponent::GetClickOptions(ClickOption **DefaultOptOut, TArray<ClickOption> *OptionsOut)
{
	// Only have option to remove
	OptionsOut->Emplace(FName(*FString::Printf(TEXT("Pickup"))), [this]() {
		if (!this->ParentPlatform || !this->ParentPlatform->GetShop())
		{
			return;
		}

		AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		ULogicalItemStaticComponent *ItemComponent = Cast<ULogicalItemStaticComponent>(LogicalComponent);
		if (Controller && ItemComponent)
		{
			// TODO maybe popup a context menu to 'take' or 'move' or maybe 'use'?
			APlayerCharacter *Character = Cast<APlayerCharacter>(Controller->GetCharacter());
			if (nullptr == Character->GetInventory()->Execute_AddItem(Character->GetInventory(), UItem::MakeItemEx(Character, ItemComponent->GetItemType())))
			{
				this->LogicalComponent->GetParentPlatform()->RemoveComponent(this->LogicalComponent);
				this->ParentPlatform->RemoveComponent(this);
				this->GetWorld()->DestroyActor(this);
			}
		}

		if (Controller)
		{
			Controller->GetHudManager()->SetScreen(nullptr); // Close screen on click
		}
	});

	*DefaultOptOut = &((*OptionsOut)[0]);

	return true;
}