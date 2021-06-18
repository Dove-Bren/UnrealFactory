#include "PlatformMine.h"

#include "Engine/StaticMesh.h"

#include "Factory/Logical/LogicalPlatformMine.h"

#define WALL3_LENGTH 500
#define FLOOR_LENGTH 100

// TODO cana this stuff in constructor be put into base class? And these display platforms can just set up meshes and materials?

UPlatformMine::UPlatformMine() : UPlatform()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> FloorMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> WallMesh;
		FConstructorStatics()
			: FloorMesh(TEXT("/Game/Factory/Meshes/Environment/Interior/Dungeon/floor_01.floor_01"))
			, WallMesh(TEXT("/Game/Factory/Meshes/Environment/Interior/Dungeon/wall_03.wall_03"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	Mesh->SetStaticMesh(ConstructorStatics.FloorMesh.Get());
	Mesh->SetRelativeScale3D(FVector(MINE_FLOOR_WIDTH, MINE_FLOOR_HEIGHT, 1.f));

	// Build walls around floor
	float ActLen = (float) (MINE_FLOOR_WIDTH * FLOOR_LENGTH) / (float) WALL3_LENGTH;
	int Len = FMath::RoundToPositiveInfinity(ActLen);
	float PartialAmt = (ActLen - (int)ActLen);
	float HalfOffsetH = ((float)(MINE_FLOOR_WIDTH * FLOOR_LENGTH) / 2.f);
	float HalfOffsetV = ((float)(MINE_FLOOR_HEIGHT * FLOOR_LENGTH) / 2.f);
	int Num = 0;

	// X top (0 offset or rotation)
	for (int i = 0; i < Len; i++) // Note starts at 1 cause of unroll
	{
		float Partial = 1.f;
		if (i + 1 > ActLen)
		{
			// Next int is going to be too big. We have a fraction
			Partial = PartialAmt;
		}
		FName Name = *FString::Printf(TEXT("Wall_%d"), Num++);
		UStaticMeshComponent *Wall = CreateDefaultSubobject<UStaticMeshComponent>(Name);
		Wall->SetStaticMesh(ConstructorStatics.WallMesh.Get());
		Wall->SetRelativeLocation(FVector(-HalfOffsetH + (i * WALL3_LENGTH), -HalfOffsetV, 0));
		Wall->SetRelativeScale3D(FVector(Partial, 1.f, 1.f));
		Wall->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		Wall->AttachToComponent(WallsParent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	// X bottom (-y scale)
	for (int i = 0; i < Len; i++)
	{
		float Partial = 1.f;
		if (i + 1 > ActLen)
		{
			// Next int is going to be too big. We have a fraction
			Partial = PartialAmt;
		}
		FName Name = *FString::Printf(TEXT("Wall_%d"), Num++);
		UStaticMeshComponent *Wall = CreateDefaultSubobject<UStaticMeshComponent>(Name);
		Wall->SetStaticMesh(ConstructorStatics.WallMesh.Get());
		Wall->SetRelativeLocation(FVector(-HalfOffsetH + (i * WALL3_LENGTH), +HalfOffsetV, 0));
		Wall->SetRelativeScale3D(FVector(Partial, -1.f, 1.f));
		Wall->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		Wall->AttachToComponent(WallsParent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	// Y left (90 yaw, mirror y (so origin is still top))
	for (int i = 0; i < Len; i++)
	{
		float Partial = 1.f;
		if (i + 1 > ActLen)
		{
			// Next int is going to be too big. We have a fraction
			Partial = PartialAmt;
		}
		FName Name = *FString::Printf(TEXT("Wall_%d"), Num++);
		UStaticMeshComponent *Wall = CreateDefaultSubobject<UStaticMeshComponent>(Name);
		Wall->SetStaticMesh(ConstructorStatics.WallMesh.Get());
		Wall->SetRelativeLocation(FVector(-HalfOffsetH, -HalfOffsetV + (i * WALL3_LENGTH), 0));
		Wall->SetRelativeScale3D(FVector(Partial, -1.f, 1.f));
		Wall->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		Wall->AttachToComponent(WallsParent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	// Y left (90 yaw)
	for (int i = 0; i < Len; i++)
	{
		float Partial = 1.f;
		if (i + 1 > ActLen)
		{
			// Next int is going to be too big. We have a fraction
			Partial = PartialAmt;
		}
		FName Name = *FString::Printf(TEXT("Wall_%d"), Num++);
		UStaticMeshComponent *Wall = CreateDefaultSubobject<UStaticMeshComponent>(Name);
		Wall->SetStaticMesh(ConstructorStatics.WallMesh.Get());
		Wall->SetRelativeLocation(FVector(+HalfOffsetH, -HalfOffsetV + (i * WALL3_LENGTH), 0));
		Wall->SetRelativeScale3D(FVector(Partial, 1.f, 1.f));
		Wall->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		Wall->AttachToComponent(WallsParent, FAttachmentTransformRules::KeepRelativeTransform);
	}
}