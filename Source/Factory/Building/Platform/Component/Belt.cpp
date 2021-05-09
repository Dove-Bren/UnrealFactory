#include "Belt.h"

static UMaterial *Belt1Mat;
static UStaticMesh *UnconnectedMesh;
static UStaticMesh *BeginMesh;
static UStaticMesh *BeginExtendedMesh;
static UStaticMesh *EndMesh;
static UStaticMesh *MiddleMesh;
static UStaticMesh *MiddleExtendedMesh;

ABelt::ABelt()
{
	// Structure to hold one-time initialization
	struct FMeshStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UMaterial> Belt1Mat;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> UnconnectedMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BeginMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BeginExtendedMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> EndMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> MiddleMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> MiddleExtendedMesh;
		FMeshStatics()
			: Belt1Mat(TEXT("/Game/Puzzle/Meshes/Machines/Belt1.Belt1"))
			, UnconnectedMesh(TEXT("/Game/Puzzle/Meshes/Machines/conveyor_standalone.conveyor_standalone"))
			, BeginMesh(TEXT("/Game/Puzzle/Meshes/Machines/conveyor_begin.conveyor_begin"))
			, BeginExtendedMesh(TEXT("/Game/Puzzle/Meshes/Machines/conveyor_begin_ext.conveyor_begin_ext"))
			, EndMesh(TEXT("/Game/Puzzle/Meshes/Machines/conveyor_end.conveyor_end"))
			, MiddleMesh(TEXT("/Game/Puzzle/Meshes/Machines/conveyor_middle.conveyor_middle"))
			, MiddleExtendedMesh(TEXT("/Game/Puzzle/Meshes/Machines/conveyor_middle_ext.conveyor_middle_ext"))
		{
		}
	};
	static FMeshStatics MeshStatics;

	Belt1Mat = MeshStatics.Belt1Mat.Get();
	UnconnectedMesh = MeshStatics.UnconnectedMesh.Get();
	BeginMesh = MeshStatics.BeginMesh.Get();
	BeginExtendedMesh = MeshStatics.BeginExtendedMesh.Get();
	EndMesh = MeshStatics.EndMesh.Get();
	MiddleMesh = MeshStatics.MiddleMesh.Get();
	MiddleExtendedMesh = MeshStatics.MiddleExtendedMesh.Get();

	RefreshMesh();
}

void ABelt::RefreshMesh()
{
	this->Mesh->SetStaticMesh(UnconnectedMesh);
	this->Mesh->SetMaterial(0, Belt1Mat);
}