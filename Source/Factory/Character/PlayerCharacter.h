#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

#include "PlayerCharacter.generated.h"

UCLASS()
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		class AFactoryBlock* CurrentBlockFocus;

	UPROPERTY(EditAnywhere)
		UCameraComponent *Camera;

private:



};