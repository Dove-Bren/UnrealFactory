#pragma once

#include "CoreMinimal.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"

#include "Factory/Clickable.h"
#include "Factory/Character/FactoryPlayerController.h"

#include "PlacingActor.generated.h"

UCLASS(BlueprintType)
class APlacingActor : public AClickableActor
{
	GENERATED_BODY()

public:

	void Setup(AFactoryPlayerController *Controller, UStaticMesh *Mesh);

	//static APlacingActor *Make(AFactoryPlayerController *Controller, UStaticMesh *Mesh) { APlacingActor *Actor = NewObject<APlacingActor>(Controller); Actor->Setup(Controller, Mesh); return Actor; }

protected:

	APlacingActor();
	virtual ~APlacingActor() = default;

	

	virtual void OnClick(FKey ButtonPressed) override;

	// Main mesh component.
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *Mesh;

	// Controller that spawned this placer. May be null if called early.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AFactoryPlayerController *Controller;

private:


};