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
			: Belt1Mat(TEXT("/Game/Factory/Meshes/Machines/Belt1.Belt1"))
			, UnconnectedMesh(TEXT("/Game/Factory/Meshes/Machines/conveyor_standalone.conveyor_standalone"))
			, BeginMesh(TEXT("/Game/Factory/Meshes/Machines/conveyor_begin.conveyor_begin"))
			, BeginExtendedMesh(TEXT("/Game/Factory/Meshes/Machines/conveyor_begin_ext.conveyor_begin_ext"))
			, EndMesh(TEXT("/Game/Factory/Meshes/Machines/conveyor_end.conveyor_end"))
			, MiddleMesh(TEXT("/Game/Factory/Meshes/Machines/conveyor_middle.conveyor_middle"))
			, MiddleExtendedMesh(TEXT("/Game/Factory/Meshes/Machines/conveyor_middle_ext.conveyor_middle_ext"))
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

	Refresh();
}

void ABelt::Refresh()
{
	APlatformComponent::Refresh();

	// Remember: "Ext" are longer to connect to the sides of conveyer belts rather
	// then the proper back of a belt (or the side of a machine)
	UStaticMesh *BeltMesh = UnconnectedMesh;

	if (LogicalBelt)
	{
		bool bForward = !!LogicalBelt->GetReceivingHandler();
		bool bBackwards = !!LogicalBelt->GetRearProducerHandler();
		bool bExtended = LogicalBelt->GetIsExtended();

		if (!bForward)
		{
			BeltMesh = bBackwards ? EndMesh : UnconnectedMesh;
		}
		else if (!bBackwards)
		{
			BeltMesh = (bExtended ? BeginExtendedMesh : BeginMesh);
		}
		else
		{
			BeltMesh = (bExtended ? MiddleExtendedMesh : MiddleMesh);
		}
	}

	this->Mesh->SetStaticMesh(BeltMesh);
	this->Mesh->SetMaterial(0, Belt1Mat);
}