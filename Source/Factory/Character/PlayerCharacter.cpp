#include "PlayerCharacter.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#include "Shop.h"
#include "Platform.h"

#define SPEED_WALK 400
#define SPEED_RUN 800

APlayerCharacter::APlayerCharacter()
{
	/*UCapsuleComponent *Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = Capsule;*/

	USpringArmComponent *CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->TargetArmLength = 1000.0;
	CameraBoom->SetRelativeRotation(FRotator(-60.0f, 45.0f, 0.0f));
	CameraBoom->bInheritPitch = false;
	CameraBoom->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.8f;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCharacter::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	Super::CalcCamera(DeltaTime, OutResult);

	//OutResult.Rotation = FRotator(0.0f, -90.0f, 0.0f);
}

void APlayerCharacter::SetShop(const AShop *Shop, EGamePlatform Platform, bool bTeleport)
{
	this->CurrentShop = Shop;
	this->CurrentPlatform = Platform;

	// Do the actual move if requested
	if (bTeleport)
	{
		FVector Loc = Shop->GetActorLocation() + FVector(100, 0, GetPlatformOffset(Platform) + 5);
		SetActorLocation(Loc, false);
	}

	ResetAbilities();
}

void APlayerCharacter::ResetAbilities()
{
	if (!CurrentShop)
	{
		// Outside
		bCanSprint = true;
		bCanJump = false;
	}
	else
	{
		switch (CurrentPlatform)
		{
		case EGamePlatform::SHOP:
			bCanSprint = false;
			bCanJump = false;
			break;
		case EGamePlatform::FACTORY:
			bCanSprint = true;
			bCanJump = false;
			break;
		case EGamePlatform::MINE:
		default:
			bCanSprint = true;
			bCanJump = true;
			break;
		}
	}
}