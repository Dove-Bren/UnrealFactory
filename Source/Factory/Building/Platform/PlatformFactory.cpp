#include "PlatformFactory.h"

UPlatformFactory::UPlatformFactory()
{
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

	Mesh->SetStaticMesh(MeshStatics.FloorMesh.Get());
	Mesh->SetMaterial(0, MeshStatics.FloorMat.Get());
	Mesh->SetRelativeScale3D(FVector(FloorWidth, FloorHeight, 1.f));

	this->WallMesh = MeshStatics.WallMesh.Get();
	this->FloorMesh = MeshStatics.FloorMesh.Get();
	this->FloorMat = MeshStatics.FloorMat.Get();
}