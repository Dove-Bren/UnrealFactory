#include "Ladder.h"

#include "FactoryPlayerController.h"
#include "PlayerCharacter.h"
#include "Platform.h"
#include "Shop.h"

ALadder::ALadder() : AClickableActor()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> LadderMesh;
		//ConstructorHelpers::FObjectFinderOptional<UStaticMesh> HoleMesh;
		FConstructorStatics()
			: LadderMesh(TEXT("/Game/Puzzle/Meshes/Environment/Interior/ladder.ladder"))
			//, WallMesh(TEXT("/Game/Puzzle/Meshes/Environment/Interior/Dungeon/wall_03.wall_03"))
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

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetRelativeLocation(FVector(25.f, 0.f, 0.f));
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

	APlayerCharacter *ControlledCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (ControlledCharacter)
	{
		// TODO show some UI and let them select up or down depending on what platform we're on
		// For now, cycle
		EGamePlatform DestPlatform = EGamePlatform::FACTORY;
		const TMap<EGamePlatform, UPlatform*> & Platforms = Platform->GetShop()->GetPlatforms();
		switch (Platform->GetType())
		{
		case EGamePlatform::SHOP:
			DestPlatform = EGamePlatform::FACTORY;
			break;
		case EGamePlatform::FACTORY:
			DestPlatform = EGamePlatform::MINE;
			break;
		case EGamePlatform::MINE:
		default:
			DestPlatform = EGamePlatform::SHOP;
			break;
		}

		FVector Loc = Platform->GetShop()->GetActorLocation() + FVector(100, 0, GetPlatformOffset(DestPlatform) + 5);
		ControlledCharacter->SetActorLocation(Loc, false);
	}
}
