#pragma once

// 

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"

#include "Factory/Clickable.h"
#include "Factory/GameEnums.h"

#include "Ladder.generated.h"

typedef class UPlatform UPlatform;

UCLASS(Blueprintable)
class ALadder : public AClickableActor
{
	GENERATED_BODY()
private:

	// Main mesh component
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *Mesh;

	// Encompassing collision capsule for better highlighting
	UPROPERTY(EditAnywhere)
	UCapsuleComponent *Capsule;

	// Layer the ladder is on
	UPROPERTY(VisibleAnywhere)
	UPlatform *Platform;

protected:
	virtual void OnClick(FKey ButtonPressed) override;

public:

	ALadder();

	void SetPlatform(UPlatform *PlatformParent) { this->Platform = PlatformParent; };
};