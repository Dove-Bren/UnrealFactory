#include "ItemActor.h"

#include "Factory/FactorySingletons.h"
#include "Factory/Character/PlayerCharacter.h"

AItemActor::AItemActor() : AActor()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//Mesh->SetGenerateOverlapEvents(false);
	//Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetRelativeScale3D(FVector(.75f, .75f, .75f));
	//Mesh->SetRelativeRotation(FRotator(0.f, 0.f, 15.f));
	this->RootComponent = Mesh;

	this->MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->SetUpdatedComponent(Mesh);
	MovementComponent->bShouldBounce = false;
	MovementComponent->ProjectileGravityScale = 1;
	MovementComponent->MaxSpeed = 1000;
	MovementComponent->InitialSpeed = 1000;

	this->SetActorEnableCollision(true);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);
	//Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	PrimaryActorTick.bCanEverTick = true;
	OnActorBeginOverlap.AddDynamic(this, &AItemActor::OnActorOverlap);
}

void AItemActor::Setup(UItem *ItemIn)
{
	this->Item = ItemIn;
	this->Mesh->SetStaticMesh(Item ? Item->GetType()->GetMesh() : nullptr);
	this->SpawnTicks = UFactorySingletons::GetInstance(GetWorld())->TotalTickCount;
}

void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO slow raise and lower like MC?
	TArray<AActor*> Characters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), Characters);

	// Check for flying towards someone and/or pickup
	if (UFactorySingletons::GetInstance(GetWorld())->TotalTickCount - SpawnTicks > 20)
	{
		// Instead of going for first, got for closest
		const float MaxDist = 400;
		APlayerCharacter *Closest = nullptr;
		double Dist = 0.0;
		for (AActor* Actor : Characters)
		{
			APlayerCharacter *Character = Cast<APlayerCharacter>(Actor);
			if (!Character)
			{
				continue;
			}

			double CurDist = GetDistanceTo(Character);
			if ((!Closest || Dist > CurDist)
				&& CurDist < MaxDist // TODO add pickup reach for characters?
				&& Character->GetInventory()->Execute_CanFit(Character->GetInventory(), this->Item)
				)
			{
				Closest = Character;
				Dist = CurDist;
			}
		}

		if (Closest)
		{
			//this->MovementComponent->HomingTargetComponent = Closest->GetRootComponent();
			//float Speed = FMath::Min(
			//	Dist, // Never make speed faster than actual distance between the two actors
			//	
			//);
			float Speed = 200.0f * (1.0f - (Dist / MaxDist)); // Linear value between 0 and 100 based on dist
			FVector Velocity = (Closest->GetActorLocation() - this->GetActorLocation());
			Velocity.Normalize();
			Velocity *= Speed;

			MovementComponent->SetUpdatedComponent(Mesh);
			this->MovementComponent->Velocity = Velocity;
			this->MovementComponent->UpdateComponentVelocity();
		}
	}
}

void AItemActor::OnActorOverlap(AActor *Actor, AActor *OtherActor)
{
	APlayerCharacter *Character = Cast<APlayerCharacter>(OtherActor);
	if (Character && Character->GetInventory()->Execute_CanFit(Character->GetInventory(), this->Item))
	{
		this->Item = Character->GetInventory()->Execute_AddItem(Character->GetInventory(), this->Item);
		if (!ITEM_EXISTS(this->Item))
		{
			this->Destroy();
			return;
		}
	}
}

//static
AItemActor *AItemActor::SpawnItemActor(UWorld *World, UItem *Item, FVector Location)
{
	FActorSpawnParameters Params;
	
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Move item up a bit for launching
	Location.Z += 50;

	//ABelt *WorldComp = Platform->GetWorld()->SpawnActor<ABelt>(SpawnLoc, GetRotationFromDirection(Direction), MakeSpawnParams());
	AItemActor *Actor = World->SpawnActor<AItemActor>(Location, FRotator(0, 0, 0), Params);
	if (Actor)
	{
		Actor->Setup(Item);
		FVector2D HMotion = FMath::RandPointInCircle(300.0f);
		Actor->MovementComponent->Velocity = FVector(HMotion.X, HMotion.Y, 400);
	}
	return Actor;
}