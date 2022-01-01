#pragma once

// Ladder world object that players click on to move between platforms

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"

#include "Factory/Building/Platform/Component/PlatformComponent.h"
#include "Factory/GameEnums.h"

#include "Ladder.generated.h"

typedef class UPlatform UPlatform;

UCLASS(Blueprintable)
class ALadder : public APlatformComponent
{
	GENERATED_BODY()
private:

	//// Main mesh component
	//UPROPERTY(EditAnywhere)
	//UStaticMeshComponent *Mesh;

	// Encompassing collision capsule for better highlighting
	UPROPERTY(EditAnywhere)
	UCapsuleComponent *Capsule;

	//// Layer the ladder is on
	//UPROPERTY(VisibleAnywhere)
	//UPlatform *Platform;

	TSubclassOf<class UPopupMenuWidget> MenuWidgetClass;

protected:
	virtual bool GetClickOptions(ClickOption **DefaultOptOut, TArray<ClickOption> *OptionsOut) override;
	virtual TSubclassOf<class UPopupMenuWidget> GetMenuWidgetClass() override { return MenuWidgetClass; }

	virtual bool ShouldHighlight_Implementation() override { return true; }

public:

	ALadder();
	virtual ~ALadder() = default;

	//void SetPlatform(UPlatform *PlatformParent) { this->Platform = PlatformParent; };
};