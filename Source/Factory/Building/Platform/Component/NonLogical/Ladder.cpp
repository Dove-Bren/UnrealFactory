#include "Ladder.h"

#include "Misc/EnumRange.h"

#include "Factory/Character/FactoryPlayerController.h"
#include "Factory/Character/PlayerCharacter.h"
#include "Factory/Building/Platform/Platform.h"
#include "Factory/Building/Shop.h"

ALadder::ALadder() : APlatformComponent()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> LadderMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> HoleMesh;
		ConstructorHelpers::FClassFinder<UPopupMenuWidget> PopupWidget;
		FConstructorStatics()
			: LadderMesh(TEXT("/Game/Factory/Meshes/Environment/Interior/ladder.ladder"))
			, HoleMesh(TEXT("/Game/Factory/Meshes/Environment/Interior/ladder_hole.ladder_hole"))
			, PopupWidget(TEXT("WidgetBlueprint'/Game/Factory/UI/PopupMenu/PopupSelectionMenu'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LadderMesh"));
	Mesh->SetStaticMesh(ConstructorStatics.LadderMesh.Get());
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 2.f));
	Mesh->SetRelativeRotation(FRotator(0.f, 0.f, 15.f));
	this->RootComponent = Mesh;

	UStaticMeshComponent *HoleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hole"));
	HoleMesh->SetStaticMesh(ConstructorStatics.HoleMesh.Get());
	HoleMesh->SetGenerateOverlapEvents(false);
	HoleMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	HoleMesh->SetRelativeLocation(FVector(0.f, 0.f, 1.f));
	HoleMesh->SetWorldRotation(FRotator(0.f, 0.f, 0.f));
	HoleMesh->SetUsingAbsoluteRotation(true);
	//HoleMesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 2.f));
	//HoleMesh->SetRelativeRotation(FRotator(0.f, 0.f, 15.f));
	HoleMesh->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetRelativeLocation(FVector(25.f, 0.f, 90.f));
	Capsule->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	Capsule->SetCapsuleSize(30.f, 120.f);
	Capsule->SetGenerateOverlapEvents(true);
	Capsule->SetCollisionObjectType(ECC_Pawn);
	Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	if (ConstructorStatics.PopupWidget.Succeeded())
	{
		this->MenuWidgetClass = ConstructorStatics.PopupWidget.Class;
	}
	//Mesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
}

bool ALadder::GetClickOptions(ClickOption **DefaultOptOut, TArray<ClickOption> *OptionsOut)
{
	if (!this->ParentPlatform || !this->ParentPlatform->GetShop())
	{
		return false;
	}

	// Figure out which we should go to by default
	EGamePlatform DefaultNextPlatform = EGamePlatform::FACTORY;
	switch (ParentPlatform->GetType())
	{
	case EGamePlatform::STORE:
		DefaultNextPlatform = EGamePlatform::FACTORY;
		break;
	case EGamePlatform::FACTORY:
		DefaultNextPlatform = EGamePlatform::MINE;
		break;
	case EGamePlatform::MINE:
	default:
		DefaultNextPlatform = EGamePlatform::STORE;
		break;
	}

	// Add all options -- noting which one is default
	for (EGamePlatform PlatformType : TEnumRange<EGamePlatform>())
	{
		if (PlatformType == ParentPlatform->GetType())\
		{
			// Skip current platform
			continue;
		}

		OptionsOut->Emplace(GetPlatformName(PlatformType), [this, PlatformType]() {

			AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			if (Controller)
			{
				Controller->GoToShop(ParentPlatform->GetShop(), PlatformType, true);
			}
		});

		if (PlatformType == DefaultNextPlatform)
		{
			*DefaultOptOut = &OptionsOut->Last();
		}
	}

	return true;
}