#include "Bin.h"

#include "Factory/Character/FactoryPlayerController.h"
#include "Factory/UI/HUDManager.h"

#include "Factory/Logical/Inventory/ItemType.h"

#include "Blueprint/UserWidget.h"

ABin::ABin() : AClickablePlatformComponent()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FClassFinder<UFactoryInventoryScreen> Screen;
		FConstructorStatics()
			: Screen(TEXT("WidgetBlueprint'/Game/Factory/UI/Screens/Screen_GenericInventory'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	if (ConstructorStatics.Screen.Succeeded())
	{
		this->ScreenClass = ConstructorStatics.Screen.Class;
	}

	Content1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ContentDisp1"));
	Content2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ContentDisp2"));
	Content3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ContentDisp3"));
	Content4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ContentDisp4"));
	Content5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ContentDisp5"));
	Content6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ContentDisp6"));
	Content7 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ContentDisp7"));

	Content1->SetRelativeLocation(FVector(-25, -25, 0));
	Content2->SetRelativeLocation(FVector(20, 20, 0));
	Content3->SetRelativeLocation(FVector(-5, -25, 0));
		Content3->SetRelativeRotation(FRotator(0, 90, -20));
	Content4->SetRelativeLocation(FVector(-20, 45, 0));
		Content4->SetRelativeRotation(FRotator(0, -180, 90));
	Content5->SetRelativeLocation(FVector(20, 0, 50));
	Content6->SetRelativeLocation(FVector(-20, -10, 65));
		Content6->SetRelativeRotation(FRotator(0, 90, 0));
	Content7->SetRelativeLocation(FVector(15, 0, 85));
		Content7->SetRelativeRotation(FRotator(30, 0, 0));

	for (UStaticMeshComponent *comp : { Content1, Content2, Content3, Content4, Content5, Content6, Content6 })
	{
		comp->SetVisibility(false);
		comp->SetRelativeScale3D(FVector(.5, .5, .5));
		comp->AttachToComponent(this->Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void ABin::UpdateContentDisplay()
{
	float Perc = (LogicalBin ? LogicalBin->GetFullness() : 0);
	Content1->SetVisibility(Perc > 0.0f);
	Content2->SetVisibility(Perc >= 0.1f);
	Content3->SetVisibility(Perc >= 0.25f);
	Content4->SetVisibility(Perc >= 0.35f);
	Content5->SetVisibility(Perc >= 0.5f);
	Content6->SetVisibility(Perc >= 0.6f);
	Content7->SetVisibility(Perc >= 0.75f);

	if (Perc > 0)
	{
		UItemType *Type = LogicalBin->GetItemType();
		UStaticMesh *ItemMesh = Type ? Type->GetMesh() : nullptr;
		if (ItemMesh)
		{
			Content1->SetStaticMesh(ItemMesh);
			Content2->SetStaticMesh(ItemMesh);
			Content3->SetStaticMesh(ItemMesh);
			Content4->SetStaticMesh(ItemMesh);
			Content5->SetStaticMesh(ItemMesh);
			Content6->SetStaticMesh(ItemMesh);
			Content7->SetStaticMesh(ItemMesh);
		}
	}
}

void ABin::Refresh()
{
	if (LogicalBin)
	{
		this->Mesh->SetStaticMesh(LogicalBin->GetBinMesh());
	}
	else
	{
		Mesh->SetVisibility(false); // Might as well hide it
	}
	UpdateContentDisplay();
}

void ABin::ShopTick(EGamePhase Phase)
{
	Super::ShopTick(Phase);
	Refresh();
}

void ABin::OnClick_Implementation(FKey ButtonPressed)
{
	if (!this->ParentPlatform || !this->ParentPlatform->GetShop() || !this->LogicalBin || !this->ScreenClass)
	{
		return;
	}

	AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (Controller)
	{
		UFactoryInventoryScreen *Screen = CreateWidget<UFactoryInventoryScreen>(GetWorld()->GetFirstPlayerController(), ScreenClass);

		if (Screen)
		{
			Screen->SetInventory(LogicalBin);
			Controller->OpenScreenAt(Screen, GetActorLocation());
		}
	}
}