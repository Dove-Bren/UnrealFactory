#include "Splitter.h"

#include "Factory/FactorySingletons.h"

ASplitter::ASplitter()
{
	// Structure to hold one-time initialization
	struct FMeshStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UMaterial> Belt1Mat;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> SplitterMesh;
		FMeshStatics()
			: Belt1Mat(TEXT("/Game/Factory/Meshes/Machines/Belt1.Belt1"))
			, SplitterMesh(TEXT("/Game/Factory/Meshes/Machines/Splitters/splitter.splitter"))
		{
		}
	};
	static FMeshStatics MeshStatics;

	BeltMat = MeshStatics.Belt1Mat.Get();
	SplitterMesh = MeshStatics.SplitterMesh.Get();

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

void ASplitter::Refresh()
{
	APlatformComponent::Refresh();

	this->Mesh->SetStaticMesh(SplitterMesh);
}

void ASplitter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Instant move now
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