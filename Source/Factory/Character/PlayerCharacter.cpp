#include "PlayerCharacter.h"

#include "Animation/AnimBlueprint.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "DrawDebugHelpers.h"

#include "Factory/Building/Shop.h"
#include "Factory/Building/Platform/Platform.h"

#define SPEED_WALK 400
#define SPEED_RUN 800

APlayerCharacter::APlayerCharacter()
{
	/*UCapsuleComponent *Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = Capsule;*/

	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<USkeletalMesh> SkeletalMesh;
		ConstructorHelpers::FObjectFinderOptional<UAnimBlueprint> AnimBlueprint;
		FConstructorStatics()
			: SkeletalMesh(TEXT("/Game/Factory/Meshes/Characters/ElfArchers/ElfMan.ElfMan"))
			, AnimBlueprint(TEXT("/Game/Factory/Meshes/Characters/ElfArchers/Animations/ElfManAnim.ElfManAnim"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Adjust size of capsule
	GetCapsuleComponent()->SetCapsuleHalfHeight(97.f);

	// Set mesh
	GetMesh()->SetSkeletalMesh(ConstructorStatics.SkeletalMesh.Get());
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetAnimClass(ConstructorStatics.AnimBlueprint.Get()->GeneratedClass);
	GetMesh()->SetRelativeTransform(FTransform(FRotator(0.f, -90.f, 0.f), FVector(0.f, 0.f, -95.f)));

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

	// Lighting for the various platforms
	// Shop/Overworld
	LightShopDirectional = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("ShopLightDirectional"));
	//LightShopDirectional->SetIntensity(10.f); // Default is good
	LightShopDirectional->SetCastShadows(true);
	LightShopDirectional->SetWorldRotation(FRotator(-60.f, 0.f, 0.f));
	LightShopDirectional->SetUsingAbsoluteRotation(true);
	LightShopDirectional->SetVisibility(true);
	LightShopDirectional->SetAtmosphereSunLight(true);
	LightShopFog = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("ShopFog"));
	LightShopFog->SetVisibility(true);
	LightShopSky = CreateDefaultSubobject<USkyLightComponent>(TEXT("ShopSkyLight"));
	LightShopSky->SetVisibility(true);

	// Shop and overworld light
	LightFactoryDirectional = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("FactoryLightDirectional"));
	LightFactoryDirectional->SetIntensity(4.f);
	LightFactoryDirectional->LightColor = FColor(214, 214, 255, 255);
	LightFactoryDirectional->SetCastShadows(false);
	LightFactoryDirectional->SetWorldRotation(FRotator(-85.f, 0.f, 0.f));
	LightFactoryDirectional->SetUsingAbsoluteRotation(true);
	LightFactoryDirectional->SetAtmosphereSunLight(false);
	LightFactoryDirectional->SetVisibility(false);
	LightFactoryPoint = CreateDefaultSubobject<UPointLightComponent>(TEXT("FactoryLightRect"));
	LightFactoryPoint->SetIntensity(60000.f);
	LightFactoryPoint->SetAttenuationRadius(60000.f);
	LightFactoryPoint->SetCastShadows(false);
	LightFactoryPoint->SetRelativeTransform(FTransform(FRotator(0.f, 0.f, 0.f), FVector(0.f, 0.f, 500.f)));
	LightFactoryPoint->SetVisibility(false);
	LightFactoryPoint->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	// Mine-level lights
	LightMine1 = CreateDefaultSubobject<UPointLightComponent>(TEXT("MineLight1"));
	LightMine1->SetIntensity(60000.f);
	LightMine1->SetAttenuationRadius(1900.f);
	LightMine1->SetCastShadows(false);
	LightMine1->SetRelativeTransform(FTransform(FRotator(0.f, 0.f, 0.f), FVector(300.f, 0.f, 566.f)));
	LightMine1->SetVisibility(false);
	LightMine1->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	LightMine2 = CreateDefaultSubobject<URectLightComponent>(TEXT("MineLight2"));
	LightMine2->SetIntensity(60000.f);
	LightMine2->SetAttenuationRadius(60000.f);
	LightMine2->SetCastShadows(false);
	LightMine2->SetRelativeTransform(FTransform(FRotator(-60.f, 0.f, 0.f), FVector(-300.f, 0.f, 566.f)));
	LightMine2->SetVisibility(false);
	LightMine2->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	AutoPossessPlayer = EAutoReceiveInput::Player0;


	// RPG element defaults
	Health = MaxHealth = 100;
	Mana = MaxMana = 20;
	Level = 1;
	XP = 0;

	this->Inventory = CreateDefaultSubobject<UInventory>(TEXT("PlayerInventory"));
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Shouldn't be here. Shop registry?
#define TICKS_PER_SECOND 20.0
#define SECONDS_PER_TICK (1.0 / TICKS_PER_SECOND)
	if (this->CurrentShop && CurrentShop->GetLogicalShop())
	{
		static float SecondBuildup = 0;
		SecondBuildup += DeltaSeconds;
		while (SecondBuildup > SECONDS_PER_TICK)
		{
			SecondBuildup -= SECONDS_PER_TICK;
			CurrentShop->GetLogicalShop()->ShopTick(EGamePhase::MORNING);
		}
	}
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

void APlayerCharacter::SetShop(AShop *Shop, EGamePlatform Platform, bool bTeleport)
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
	ResetLighting();
	if (Shop)
	{
		Shop->SetPlatformVisibility(Platform);
	}
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
		case EGamePlatform::STORE:
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

void APlayerCharacter::ResetLighting()
{
	EGamePlatform LightPlatform = CurrentPlatform;
	if (!CurrentShop)
	{
		LightPlatform = EGamePlatform::STORE;
	}

	switch (LightPlatform)
	{
	case EGamePlatform::STORE:
	default:
		LightShopDirectional->SetVisibility(true);
		LightShopFog->SetVisibility(true);
		LightShopSky->SetVisibility(true);
		LightFactoryDirectional->SetVisibility(false);
		LightFactoryPoint->SetVisibility(false);
		LightMine1->SetVisibility(false);
		LightMine2->SetVisibility(false);
		break;
	case EGamePlatform::FACTORY:
		LightShopDirectional->SetVisibility(false);
		LightShopFog->SetVisibility(false);
		LightShopSky->SetVisibility(false);
		LightFactoryDirectional->SetVisibility(true);
		LightFactoryPoint->SetVisibility(true);
		LightMine1->SetVisibility(false);
		LightMine2->SetVisibility(false);
		break;
	case EGamePlatform::MINE:
		LightShopDirectional->SetVisibility(false);
		LightShopFog->SetVisibility(false);
		LightShopSky->SetVisibility(false);
		LightFactoryDirectional->SetVisibility(false);
		LightFactoryPoint->SetVisibility(false);
		LightMine1->SetVisibility(true);
		LightMine2->SetVisibility(true);
		break;
	}
}

#include "Factory/Logical/LogicalShop.h"
#include "FactoryPlayerController.h"

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// testing code
	float X = GetActorLocation().X;
	float Y = GetActorLocation().Y;
	float Z = GetActorLocation().Z;

	ULogicalShop *Shop = ULogicalShop::MakeShop(GetWorld(), FName(TEXT("Player Shop")), X, Y, Z);
	AShop *WorldShop = AShop::MakeShop(GetWorld(), Shop);

	AFactoryPlayerController *FactoryController = Cast<AFactoryPlayerController>(this->Controller);
	if (FactoryController)
	{
		FactoryController->GoToShop(WorldShop);
	}
}

UPlatform *APlayerCharacter::GetPlatform()
{
	return CurrentShop ? CurrentShop->GetPlatforms()[CurrentPlatform] : nullptr;
}