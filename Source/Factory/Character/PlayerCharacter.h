#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

#include "GameEnums.h"

#include "PlayerCharacter.generated.h"

typedef class AShop AShop;

UCLASS()
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

	void SetShop(const AShop *Shop, EGamePlatform Platform = EGamePlatform::SHOP, bool bTeleport = true);

	bool CanSprint() { return bCanSprint; }
	bool CanJump() { return bCanJump; }

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		class AFactoryBlock* CurrentBlockFocus;

	UPROPERTY(EditAnywhere)
	UCameraComponent *Camera;

	// Sets abilities as appropriate for current shop and platform
	void ResetAbilities();

	bool bCanSprint;
	bool bCanJump;

private:

	// Shop we're currently in. Can be null.
	UPROPERTY(VisibleAnywhere)
	const AShop *CurrentShop = nullptr;

	// Which platform we're on. Meaningless if CurrentShop is null.
	UPROPERTY(VisibleAnywhere)
	EGamePlatform CurrentPlatform = EGamePlatform::SHOP;

};