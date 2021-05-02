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

	this->FloorWidth = MIN_FACTORY_WIDTH;
	this->FloorHeight = MIN_FACTORY_HEIGHT;

	Mesh->SetStaticMesh(MeshStatics.FloorMesh.Get());
	Mesh->SetMaterial(0, MeshStatics.FloorMat.Get());
	Mesh->SetRelativeScale3D(FVector(FloorWidth, FloorHeight, 1.f));

	this->WallMesh = MeshStatics.WallMesh.Get();
	this->FloorMesh = MeshStatics.FloorMesh.Get();
	this->FloorMat = MeshStatics.FloorMat.Get();
}

void UPlatformFactory::SpawnWalls(float Width, float Height)
{

	// Build walls. Passed in W/H are in 'cells'
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

	// Stretch floor
	Mesh->SetRelativeScale3D(FVector(Width, Height, 1.f));

	// Recreate base object
	if (Walls)
	{
		Walls->DestroyComponent();
	}
	Walls = NewObject<USceneComponent>();
	Walls->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	// X top (0 offset or rotation)
	for (int i = 0; i < LenW; i++) // Note starts at 1 cause of unroll
	{
		float Partial = 1.f;
		if (i + 1 > WallsW)
		{
			// Next int is going to be too big. We have a fraction
			Partial = PartialAmtW;
		}
		UStaticMeshComponent *Wall = NewObject<UStaticMeshComponent>();
		Wall->SetStaticMesh(WallMesh);
		Wall->SetRelativeLocation(FVector(-HalfOffsetH + (i * WALL3_LENGTH), -HalfOffsetV, 0));
		Wall->SetRelativeScale3D(FVector(Partial, 1.f, 1.f));
		Wall->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		Wall->AttachToComponent(Walls, FAttachmentTransformRules::KeepRelativeTransform);
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
		UStaticMeshComponent *Wall = NewObject<UStaticMeshComponent>();
		Wall->SetStaticMesh(WallMesh);
		Wall->SetRelativeLocation(FVector(-HalfOffsetH + (i * WALL3_LENGTH), +HalfOffsetV, 0));
		Wall->SetRelativeScale3D(FVector(Partial, -1.f, 1.f));
		Wall->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		Wall->AttachToComponent(Walls, FAttachmentTransformRules::KeepRelativeTransform);
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
		UStaticMeshComponent *Wall = NewObject<UStaticMeshComponent>();
		Wall->SetStaticMesh(WallMesh);
		Wall->SetRelativeLocation(FVector(-HalfOffsetH, -HalfOffsetV + (i * WALL3_LENGTH), 0));
		Wall->SetRelativeScale3D(FVector(Partial, -1.f, 1.f));
		Wall->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		Wall->AttachToComponent(Walls, FAttachmentTransformRules::KeepRelativeTransform);
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
		UStaticMeshComponent *Wall = NewObject<UStaticMeshComponent>();
		Wall->SetStaticMesh(WallMesh);
		Wall->SetRelativeLocation(FVector(+HalfOffsetH, -HalfOffsetV + (i * WALL3_LENGTH), 0));
		Wall->SetRelativeScale3D(FVector(Partial, 1.f, 1.f));
		Wall->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		Wall->AttachToComponent(Walls, FAttachmentTransformRules::KeepRelativeTransform);
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