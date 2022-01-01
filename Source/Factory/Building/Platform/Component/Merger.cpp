#include "Merger.h"

#include "Factory/FactorySingletons.h"

AMerger::AMerger()
{
	// Structure to hold one-time initialization
	struct FMeshStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UMaterial> Belt1Mat;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> MergerMesh;
		FMeshStatics()
			: Belt1Mat(TEXT("/Game/Factory/Meshes/Machines/Belt1.Belt1"))
			, MergerMesh(TEXT("/Game/Factory/Meshes/Machines/Mergers/merger.merger"))
		{
		}
	};
	static FMeshStatics MeshStatics;

	BeltMat = MeshStatics.Belt1Mat.Get();
	MergerMesh = MeshStatics.MergerMesh.Get();

	ItemMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComp"));
	ItemMeshComp->SetRelativeScale3D(FVector(.5, .5, .5));
	ItemMeshComp->SetWorldRotation(FRotator(0, 20, 0));
	ItemMeshComp->SetUsingAbsoluteRotation(true);
	ItemMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ItemMeshComp->SetVisibility(false);
	ItemMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	PostProcessComp = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComp"));
	//PostProcessComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	PostProcessComp->AttachToComponent(ItemMeshComp, FAttachmentTransformRules::KeepRelativeTransform);
	PostProcessComp->Settings.bOverride_MotionBlurAmount = true;
	PostProcessComp->Settings.MotionBlurAmount = 0.0f;

	PrimaryActorTick.bCanEverTick = true;

	Refresh();
}

void AMerger::Refresh()
{
	APlatformComponent::Refresh();

	this->Mesh->SetStaticMesh(MergerMesh);
}

void AMerger::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// For now, not going to move things through the merger
	//if (LogicalBelt && UItem::ItemExists(LogicalBelt->GetItem()))
	//{
	//	ItemMeshComp->SetVisibility(true);
	//	ItemMeshComp->SetStaticMesh(LogicalBelt->GetItem()->GetType()->GetMesh());

	//	// Set position along the belt
	//	// If first half, rotate towards input
	//	float Progress = LogicalBelt->GetItemProgress(UFactorySingletons::GetInstance(GetWorld())->PartialTicks);
	//	FVector Loc(0, 0, 20);
	//	if (Progress < 0.5f)
	//	{
	//		EDirection FromDir = LogicalBelt->GetLastInputDirection();

	//		// Rotate so act as if this is an eastward belt
	//		EDirection Iter = LogicalBelt->GetDirection();
	//		while (Iter != EDirection::EAST)
	//		{
	//			Iter = RotateDirection(Iter);
	//			FromDir = RotateDirection(FromDir);
	//		}

	//		FVector Offset = GetDirectionOffset(FromDir) * 100.0f;
	//		Loc = Offset * (0.5f - Progress);
	//		Loc.Z = 20;
	//	}
	//	else
	//	{
	//		Loc.X = -50 + (Progress * 100);
	//	}
	//	//PostProcessComp->Settings.bOverride_MotionBlurAmount = false;
	//	ItemMeshComp->SetRelativeLocation(Loc);
	//}
	//else
	//{
	//	if (ItemMeshComp->IsVisible())
	//	{
	//		ItemMeshComp->SetVisibility(false);
	//		PostProcessComp->Settings.bOverride_MotionBlurAmount = true;
	//		ItemMeshComp->SetRelativeLocation(FVector(-50, 0, 20), false, nullptr, ETeleportType::ResetPhysics);
	//	}
	//}
}