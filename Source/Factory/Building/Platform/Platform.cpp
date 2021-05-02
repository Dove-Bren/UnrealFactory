#include "Platform.h"

#include "Shop.h"

#define WALL3_LENGTH 500
#define FLOOR_LENGTH 100

#define WORLD_TO_CELL(X) ((int32) X / CELL_SIZE)

UPlatform::UPlatform()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	Mesh->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	Grid = CreateDefaultSubobject<UComponentLayout>(TEXT("Grid"));

	WallsParent = CreateDefaultSubobject<USceneComponent>(TEXT("WallsParent"));
	WallsParent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	this->FloorWidth = MIN_PLATFORM_WIDTH * 2;
	this->FloorHeight = MIN_PLATFORM_HEIGHT * 2;
}

void UPlatform::AttachToShop(EGamePlatform Type, AShop *Shop)
{
	this->PlatformType = Type;
	this->ShopParent = Shop;

	this->AttachToComponent(Shop->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}

void UPlatform::BeginPlay()
{
	Super::BeginPlay();

	Ladder = GetWorld()->SpawnActor<ALadder>(ALadder::StaticClass(), this->GetComponentTransform());
	Ladder->SetActorRelativeLocation(FVector(-25.f, 0.f, 0.f));
	Ladder->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	Ladder->SetPlatform(this);

	// Idk how loading and multiplayer works...
	if (!bStaticFloor)
	{
		if (this->FloorWidth < MIN_PLATFORM_WIDTH)
		{
			this->FloorWidth = MIN_PLATFORM_WIDTH;
		}
		if (this->FloorHeight < MIN_PLATFORM_HEIGHT)
		{
			this->FloorHeight = MIN_PLATFORM_HEIGHT;
		}

		SpawnWalls(this->FloorWidth, this->FloorHeight);
	}
}

void UPlatform::AddComponent(APlatformComponent *Component)
{
	// Calculate where on the grid the component should go
	int32 CellX = WORLD_TO_CELL(Component->GetActorLocation().X);
	int32 CellY = WORLD_TO_CELL(Component->GetActorLocation().Y);

	APlatformComponent *Existing = this->Grid->Insert(CellX, CellY, Component, true);
	if (Existing)
	{
		Existing->RemoveFromPlatform(this);
	}
	Component->RegisterPlatform(this);
}

void UPlatform::StartPhase(EGamePhase Phase)
{
	for (APlatformComponent *comp : Grid->GetComponents())
	{
		comp->StartPhase(Phase);
	}
}

void UPlatform::ShopTick(EGamePhase Phase)
{
	for (APlatformComponent *comp : Grid->GetComponents())
	{
		comp->ShopTick(Phase);
	}
}

void UPlatform::SpawnWalls(float Width, float Height)
{
	if (bStaticFloor)
	{
		return;
	}

	// Build Walls. Passed in W/H are in 'cells'
	float RealWidth = (Width * FLOOR_LENGTH);
	float RealHeight = (Width * FLOOR_LENGTH);
	float WallsW = (RealWidth / (float)WALL3_LENGTH);
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

void UPlatform::Resize(float Width, float Height)
{
	if (bStaticFloor)
	{
		return;
	}

	this->FloorWidth = Width;
	this->FloorHeight = Height;

	if (this->FloorWidth < MIN_PLATFORM_WIDTH)
	{
		this->FloorWidth = MIN_PLATFORM_WIDTH;
	}
	if (this->FloorHeight < MIN_PLATFORM_HEIGHT)
	{
		this->FloorHeight = MIN_PLATFORM_HEIGHT;
	}

	SpawnWalls(this->FloorWidth, this->FloorHeight);
}