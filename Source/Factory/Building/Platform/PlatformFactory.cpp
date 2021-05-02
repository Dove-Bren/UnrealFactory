#include "PlatformFactory.h"

#define WALL3_LENGTH 500
#define FLOOR_LENGTH 100

UPlatformFactory::UPlatformFactory()
{
	/*
	FloorMesh;
	UStaticMesh *WallMesh;
	UMaterial *FloorMat;
	
	
	*/


	// Structure to hold one-time initialization
	struct FMeshStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> FloorMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> WallMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> FloorMat;
		FMeshStatics()
			: FloorMesh(TEXT("/Game/Puzzle/Meshes/Environment/Interior/Dungeon/floor_01.floor_01"))
			, WallMesh(TEXT("/Game/Puzzle/Meshes/Environment/Interior/Dungeon/wall_03.wall_03"))
			, FloorMat(TEXT("/Game/Puzzle/Meshes/Environment/Interior/Dungeon/MatFactFloor.MatFactFloor"))
		{
		}
	};
	static FMeshStatics MeshStatics;

	this->FloorWidth = MIN_FACTORY_WIDTH * 2;
	this->FloorHeight = MIN_FACTORY_HEIGHT * 2;

	Mesh->SetStaticMesh(MeshStatics.FloorMesh.Get());
	Mesh->SetMaterial(0, MeshStatics.FloorMat.Get());
	Mesh->SetRelativeScale3D(FVector(FloorWidth, FloorHeight, 1.f));

	this->WallMesh = MeshStatics.WallMesh.Get();
	this->FloorMesh = MeshStatics.FloorMesh.Get();
	this->FloorMat = MeshStatics.FloorMat.Get();

	WallsParent = CreateDefaultSubobject<USceneComponent>(TEXT("WallsParent"));
	WallsParent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
}

void UPlatformFactory::SpawnWalls(float Width, float Height)
{
	// Build Walls. Passed in W/H are in 'cells'
	float RealWidth = (Width * FLOOR_LENGTH);
	float RealHeight = (Width * FLOOR_LENGTH);
	float WallsW = (RealWidth / (float) WALL3_LENGTH);
	float WallsH = (RealHeight / (float)WALL3_LENGTH);
	int LenW = FMath::RoundToPositiveInfinity(WallsW);
	int LenH = FMath::RoundToPositiveInfinity(WallsH);
	float PartialAmtW = (WallsW - (int)WallsW);
	float PartialAmtH = (WallsH - (int)WallsH);
	float HalfOffsetH = RealWidth / 2.f;
	float HalfOffsetV = RealHeight / 2.f;
	int Num = 0;
	FVector Origin = GetComponentTransform().GetLocation();

	// Stretch floor
	Mesh->SetRelativeScale3D(FVector(Width, Height, 1.f));

	// Recreate base object
	if (!WallsParent)
	{
		WallsParent = NewObject<USceneComponent>();
		WallsParent->RegisterComponent();
		WallsParent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	}
	else
	{
		// Remove all existing WallsParent
		for (UStaticMeshComponent *Component : WallComponents)
		{
			Component->DestroyComponent();
		}
		WallComponents.Empty(); // Clear
	}

	// X top (0 offset or rotation)
	for (int i = 0; i < LenW; i++) // Note starts at 1 cause of unroll
	{
		float Partial = 1.f;
		if (i + 1 > WallsW)
		{
			// Next int is going to be too big. We have a fraction
			Partial = PartialAmtW;
		}
		UStaticMeshComponent *Wall = NewObject<UStaticMeshComponent>(WallsParent, UStaticMeshComponent::StaticClass());
		Wall->SetStaticMesh(WallMesh);
		Wall->SetWorldLocation(Origin + FVector(-HalfOffsetH + (i * WALL3_LENGTH), -HalfOffsetV, 0));
		Wall->SetRelativeScale3D(FVector(Partial, 1.f, 1.f));
		Wall->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		Wall->RegisterComponent();
	}

	// X bottom (-y scale)
	for (int i = 0; i < LenW; i++)
	{
		float Partial = 1.f;
		if (i + 1 > WallsW)
		{
			// Next int is going to be too big. We have a fraction
			Partial = PartialAmtW;
		}
		UStaticMeshComponent *Wall = NewObject<UStaticMeshComponent>(WallsParent, UStaticMeshComponent::StaticClass());
		Wall->SetStaticMesh(WallMesh);
		Wall->SetWorldLocation(Origin + FVector(-HalfOffsetH + (i * WALL3_LENGTH), +HalfOffsetV, 0));
		Wall->SetRelativeScale3D(FVector(Partial, -1.f, 1.f));
		Wall->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		Wall->RegisterComponent();
	}

	// Y left (90 yaw, mirror y (so origin is still top))
	for (int i = 0; i < LenH; i++)
	{
		float Partial = 1.f;
		if (i + 1 > WallsH)
		{
			// Next int is going to be too big. We have a fraction
			Partial = PartialAmtH;
		}
		UStaticMeshComponent *Wall = NewObject<UStaticMeshComponent>(WallsParent, UStaticMeshComponent::StaticClass());
		Wall->SetStaticMesh(WallMesh);
		Wall->SetWorldLocation(Origin + FVector(-HalfOffsetH, -HalfOffsetV + (i * WALL3_LENGTH), 0));
		Wall->SetRelativeScale3D(FVector(Partial, -1.f, 1.f));
		Wall->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		Wall->RegisterComponent();
	}

	// Y left (90 yaw)
	for (int i = 0; i < LenH; i++)
	{
		float Partial = 1.f;
		if (i + 1 > WallsH)
		{
			// Next int is going to be too big. We have a fraction
			Partial = PartialAmtH;
		}
		UStaticMeshComponent *Wall = NewObject<UStaticMeshComponent>(WallsParent, UStaticMeshComponent::StaticClass());
		Wall->SetStaticMesh(WallMesh);
		Wall->SetWorldLocation(Origin + FVector(+HalfOffsetH, -HalfOffsetV + (i * WALL3_LENGTH), 0));
		Wall->SetRelativeScale3D(FVector(Partial, 1.f, 1.f));
		Wall->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		Wall->RegisterComponent();
		//Wall->AttachToComponent(WallsParent, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void UPlatformFactory::BeginPlay()
{
	Super::BeginPlay();

	// Idk how loading and multiplayer works...
	if (this->FloorWidth < MIN_FACTORY_WIDTH)
	{
		this->FloorWidth = MIN_FACTORY_WIDTH;
	}
	if (this->FloorHeight < MIN_FACTORY_HEIGHT)
	{
		this->FloorHeight = MIN_FACTORY_HEIGHT;
	}

	SpawnWalls(this->FloorWidth, this->FloorHeight);
}

void UPlatformFactory::Resize(float Width, float Height)
{
	this->FloorWidth = Width;
	this->FloorHeight = Height;

	if (this->FloorWidth < MIN_FACTORY_WIDTH)
	{
		this->FloorWidth = MIN_FACTORY_WIDTH;
	}
	if (this->FloorHeight < MIN_FACTORY_HEIGHT)
	{
		this->FloorHeight = MIN_FACTORY_HEIGHT;
	}

	SpawnWalls(this->FloorWidth, this->FloorHeight);
}