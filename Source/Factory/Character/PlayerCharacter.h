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

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

	void SetShop(AShop *Shop, EGamePlatform Platform, bool bTeleport);

	// Get current shop if any
	UFUNCTION(BlueprintCallable)
	AShop *GetShop() { return CurrentShop; }

	bool CanSprint() { return bCanSprint; }
	bool CanJump() { return bCanJump; }

	// Get the current number of health points this character has
	UFUNCTION(BlueprintCallable)
	int32 GetHealth() { return Health; }

	// Get the maximum total number of health points this character can have
	UFUNCTION(BlueprintCallable)
	int32 GetMaxHealth() { return MaxHealth; }

	// Convenience function; get percentage of available health (0f to 1f)
	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() { return (float) GetHealth() / (float) GetMaxHealth(); }

	// Get the current number of mana points this character has
	UFUNCTION(BlueprintCallable)
	int32 GetMana() { return Mana; }

	// Get the maximum total number of mana this character can have
	UFUNCTION(BlueprintCallable)
	int32 GetMaxMana() { return MaxMana; }

	// Convenience function; get percentage of available mana (0f to 1f)
	UFUNCTION(BlueprintCallable)
	float GetManaPercent() { return (float) GetMana() / (float) GetMaxMana(); }

	// Get the current number of experience points this player has
	UFUNCTION(BlueprintCallable)
	int32 GetXP() { return XP; }

	// Get the current level of the character
	UFUNCTION(BlueprintCallable)
	int32 GetLevel() { return Level; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
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

	// RPG elements

	// Current health
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	int32 Health;

	// Current maximum health amount
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	int32 MaxHealth;

	// Current mana
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	int32 Mana;

	// Current maximum mana amount
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	int32 MaxMana;

	// Current number of experience points.
	// Number is reset each time a new level is achieved.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	int32 XP;

	// Current level of the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	int32 Level;

private:

	// Shop we're currently in. Can be null.
	UPROPERTY(VisibleAnywhere)
	AShop *CurrentShop = nullptr;

	// Which platform we're on. Meaningless if CurrentShop is null.
	UPROPERTY(VisibleAnywhere)
	EGamePlatform CurrentPlatform = EGamePlatform::STORE;

};