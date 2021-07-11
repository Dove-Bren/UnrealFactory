#include "PlacingActor.h"

APlacingActor::APlacingActor()
{
	// Structure to hold one-time initialization
	struct FMeshStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> InActiveMat;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> InInactiveMat;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> OutValidMat;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> OutInvalidMat;
		FMeshStatics()
			: PlaneMesh(TEXT("/Engine/BasicShapes/Plane.Plane"))
			, InActiveMat(TEXT("/Game/Factory/Sprites/Icons/Mat_ArrowHint_In_Active.Mat_ArrowHint_In_Active"))
			, InInactiveMat(TEXT("/Game/Factory/Sprites/Icons/Mat_ArrowHint_In_Inactive.Mat_ArrowHint_In_Inactive"))
			, OutValidMat(TEXT("/Game/Factory/Sprites/Icons/Mat_ArrowHint_Out_Good.Mat_ArrowHint_Out_Good"))
			, OutInvalidMat(TEXT("/Game/Factory/Sprites/Icons/Mat_ArrowHint_Out_Bad.Mat_ArrowHint_Out_Bad"))
		{
		}
	};
	static FMeshStatics MeshStatics;

	ConnectionInInactiveMat = MeshStatics.InInactiveMat.Get();
	ConnectionInActiveMat = MeshStatics.InActiveMat.Get();
	ConnectionOutInvalidMat = MeshStatics.OutInvalidMat.Get();
	ConnectionOutValidMat = MeshStatics.OutValidMat.Get();

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//Mesh->SetGenerateOverlapEvents(false);
	//Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	//Mesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 2.f));
	//Mesh->SetRelativeRotation(FRotator(0.f, 0.f, 15.f));
	this->RootComponent = Mesh;

	this->SetActorEnableCollision(false);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	UStaticMeshComponent *OutComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EastIndicatorOutPlane"));
	UStaticMeshComponent *InComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EastIndicatorInPlane"));
	OutComp->SetStaticMesh(MeshStatics.PlaneMesh.Get());
	//OutComp->SetRelativeLocation(FVector(0, 0, 50));
	OutComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	OutComp->SetWorldRotation(GetRotationFromDirection(EDirection::EAST));
	OutComp->SetAbsolute(true, true, false);
	OutComp->SetVisibility(false);
	OutComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	InComp->SetStaticMesh(MeshStatics.PlaneMesh.Get());
	//InComp->SetRelativeLocation(FVector(100, 0, 50));
	InComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	InComp->SetWorldRotation(GetRotationFromDirection(OppositeDirection(EDirection::EAST)));
	InComp->SetAbsolute(true, true, false);
	InComp->SetVisibility(false);
	InComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	ConnectionIndicators.Add(EDirection::EAST, TPair<UStaticMeshComponent*, UStaticMeshComponent*>(InComp, OutComp));
	OutComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SouthIndicatorOutPlane"));
	InComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SouthIndicatorInPlane"));
	OutComp->SetStaticMesh(MeshStatics.PlaneMesh.Get());
	//OutComp->SetRelativeLocation(FVector(0, 0, 50));
	OutComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	OutComp->SetWorldRotation(GetRotationFromDirection(EDirection::SOUTH));
	OutComp->SetAbsolute(true, true, false);
	OutComp->SetVisibility(false);
	OutComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	InComp->SetStaticMesh(MeshStatics.PlaneMesh.Get());
	//InComp->SetRelativeLocation(FVector(0, 100, 50));
	InComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	InComp->SetWorldRotation(GetRotationFromDirection(OppositeDirection(EDirection::SOUTH)));
	InComp->SetAbsolute(true, true, false);
	InComp->SetVisibility(false);
	InComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	ConnectionIndicators.Add(EDirection::SOUTH, TPair<UStaticMeshComponent*, UStaticMeshComponent*>(InComp, OutComp));
	OutComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WestIndicatorOutPlane"));
	InComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WestIndicatorInPlane"));
	OutComp->SetStaticMesh(MeshStatics.PlaneMesh.Get());
	//OutComp->SetRelativeLocation(FVector(0, 0, 50));
	OutComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	OutComp->SetWorldRotation(GetRotationFromDirection(EDirection::WEST));
	OutComp->SetAbsolute(true, true, false);
	OutComp->SetVisibility(false);
	OutComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	InComp->SetStaticMesh(MeshStatics.PlaneMesh.Get());
	//InComp->SetRelativeLocation(FVector(-100, 0, 50));
	InComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	InComp->SetWorldRotation(GetRotationFromDirection(OppositeDirection(EDirection::WEST)));
	InComp->SetAbsolute(true, true, false);
	InComp->SetVisibility(false);
	InComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	ConnectionIndicators.Add(EDirection::WEST, TPair<UStaticMeshComponent*, UStaticMeshComponent*>(InComp, OutComp));
	OutComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NorthIndicatorOutPlane"));
	InComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NorthIndicatorInPlane"));
	OutComp->SetStaticMesh(MeshStatics.PlaneMesh.Get());
	//OutComp->SetRelativeLocation(FVector(0, 0, 50));
	OutComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	OutComp->SetWorldRotation(GetRotationFromDirection(EDirection::NORTH));
	OutComp->SetAbsolute(true, true, false);
	OutComp->SetVisibility(false);
	OutComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	InComp->SetStaticMesh(MeshStatics.PlaneMesh.Get());
	//InComp->SetRelativeLocation(FVector(0, -100, 50));
	InComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	InComp->SetWorldRotation(GetRotationFromDirection(OppositeDirection(EDirection::NORTH)));
	InComp->SetAbsolute(true, true, false);
	InComp->SetVisibility(false);
	InComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	ConnectionIndicators.Add(EDirection::NORTH, TPair<UStaticMeshComponent*, UStaticMeshComponent*>(InComp, OutComp));

	PrimaryActorTick.bCanEverTick = true;
}

void APlacingActor::Setup(AFactoryPlayerController *PlayerController, UStaticMesh *MeshIn)
{
	this->Controller = PlayerController;

	this->Mesh->SetStaticMesh(MeshIn);
}

void APlacingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Origin = GetActorLocation();
	Origin.Z += 50; // Raise half a layer

	// Keep all panes in the right spot
	for (EDirection Direction : TEnumRange<EDirection>())
	{
		TPair<UStaticMeshComponent*, UStaticMeshComponent*> Meshes = ConnectionIndicators[Direction];
		Meshes.Key->SetWorldLocation(Origin + (GetDirectionOffset(Direction) * 100));
		Meshes.Value->SetWorldLocation(Origin); // Same as actor location
	}
}

void APlacingActor::OnClick(FKey ButtonPressed)
{
	if (Controller)
	{
		Controller->ActiveItemClicked();
	}
}

void APlacingActor::UpdateConnectionInfo(const FDirectionMap<EConnectionStatus> &Inputs, const FDirectionMap<EConnectionStatus> &Outputs)
{
	// First map dictates if the plane is visible (if there's info about that connection).
	// Second map indicates whether it's good/active or bad/inactive
	for (EDirection Direction : TEnumRange<EDirection>())
	{
		EConnectionStatus InputStatus = Inputs.Get(Direction);
		EConnectionStatus OutputStatus = Outputs.Get(Direction);
		TPair<UStaticMeshComponent*, UStaticMeshComponent*> Meshes = ConnectionIndicators[Direction];

		// Input
		{
			UStaticMeshComponent *DirPlane = Meshes.Key;
			bool bUsed = InputStatus != EConnectionStatus::NO_CONNECTION;
			bool bValid = InputStatus == EConnectionStatus::ACTIVE;

			DirPlane->SetVisibility(bUsed);
			if (bUsed)
			{
				DirPlane->SetMaterial(0, bValid ? ConnectionInActiveMat : ConnectionInInactiveMat);
			}
		}


		// Output
		{
			UStaticMeshComponent *DirPlane = Meshes.Value;
			bool bUsed = OutputStatus != EConnectionStatus::NO_CONNECTION;
			bool bValid = OutputStatus == EConnectionStatus::VALID;

			DirPlane->SetVisibility(bUsed);
			if (bUsed)
			{
				DirPlane->SetMaterial(0, bValid ? ConnectionOutValidMat : ConnectionOutInvalidMat);
			}
		}
	}

}