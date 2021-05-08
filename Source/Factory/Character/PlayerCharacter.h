#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/RectLightComponent.h"
#include "Components/SkyLightComponent.h"

#include "Factory/GameEnums.h"

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

	void SetShop(const AShop *Shop, EGamePlatform Platform = EGamePlatform::STORE, bool bTeleport = true);

	bool CanSprint() { return bCanSprint; }
	bool CanJump() { return bCanJump; }

protected:

	UPROPERTY(EditAnywhere)
	UCameraComponent *Camera;

	// Shop and overworld light
	UPROPERTY(EditAnywhere)
	UDirectionalLightComponent *LightShopDirectional;
	UPROPERTY(EditAnywhere)
	USkyAtmosphereComponent *LightShopFog;
	UPROPERTY(EditAnywhere)
	USkyLightComponent *LightShopSky;

	// Shop and overworld light
	UPROPERTY(EditAnywhere)
	UDirectionalLightComponent *LightFactoryDirectional;
	UPROPERTY(EditAnywhere)
	UPointLightComponent *LightFactoryPoint;


	// Mine-level lights
	UPROPERTY(EditAnywhere)
	UPointLightComponent *LightMine1;
	UPROPERTY(EditAnywhere)
	URectLightComponent *LightMine2;


	// Sets abilities as appropriate for current shop and platform
	void ResetAbilities();

	// Sets up lighting based on the approrpriate shop and platform
	void ResetLighting();

	bool bCanSprint;
	bool bCanJump;

private:

	// Shop we're currently in. Can be null.
	UPROPERTY(VisibleAnywhere)
	const AShop *CurrentShop = nullptr;

	// Which platform we're on. Meaningless if CurrentShop is null.
	UPROPERTY(VisibleAnywhere)
	EGamePlatform CurrentPlatform = EGamePlatform::STORE;

};