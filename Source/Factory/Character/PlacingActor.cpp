#include "PlacingActor.h"

APlacingActor::APlacingActor()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//Mesh->SetGenerateOverlapEvents(false);
	//Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	//Mesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 2.f));
	//Mesh->SetRelativeRotation(FRotator(0.f, 0.f, 15.f));
	this->RootComponent = Mesh;

	this->SetActorEnableCollision(false);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void APlacingActor::Setup(AFactoryPlayerController *PlayerController, UStaticMesh *MeshIn)
{
	this->Controller = PlayerController;

	this->Mesh->SetStaticMesh(MeshIn);
}

void APlacingActor::OnClick(FKey ButtonPressed)
{
	if (Controller)
	{
		Controller->ActiveItemClicked();
	}
}