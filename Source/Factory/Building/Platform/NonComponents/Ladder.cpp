#include "Ladder.h"

#include "Factory/Character/FactoryPlayerController.h"
#include "Factory/Character/PlayerCharacter.h"
#include "Factory/Building/Platform/Platform.h"
#include "Factory/Building/Shop.h"

ALadder::ALadder() : AClickableActor()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> LadderMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> HoleMesh;
		FConstructorStatics()
			: LadderMesh(TEXT("/Game/Factory/Meshes/Environment/Interior/ladder.ladder"))
			, HoleMesh(TEXT("/Game/Factory/Meshes/Environment/Interior/ladder_hole.ladder_hole"))
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

	
	//Mesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
}

void ALadder::OnClick(FKey ButtonPressed)
{

	if (!this->Platform || !this->Platform->GetShop())
	{
		return;
	}

	AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (Controller)
	{
		// TODO show some UI and let them select up or down depending on what platform we're on
		// For now, cycle
		EGamePlatform DestPlatform = EGamePlatform::FACTORY;
		const TMap<EGamePlatform, UPlatform*> & Platforms = Platform->GetShop()->GetPlatforms();
		switch (Platform->GetType())
		{
		case EGamePlatform::STORE:
			DestPlatform = EGamePlatform::FACTORY;
			break;
		case EGamePlatform::FACTORY:
			DestPlatform = EGamePlatform::MINE;
			break;
		case EGamePlatform::MINE:
		default:
			DestPlatform = EGamePlatform::STORE;
			break;
		}

		Controller->GoToShop(Platform->GetShop(), DestPlatform, true);
	}
}
