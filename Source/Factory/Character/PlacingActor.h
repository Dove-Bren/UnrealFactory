#pragma once

#include "CoreMinimal.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"

#include "Factory/Clickable.h"
#include "Factory/DirectionFlagMap.h"
#include "Factory/Character/FactoryPlayerController.h"

#include "PlacingActor.generated.h"

UCLASS(BlueprintType)
class APlacingActor : public AClickableActor
{
	GENERATED_BODY()

public:

	void Setup(AFactoryPlayerController *Controller, UStaticMesh *Mesh);

	//static APlacingActor *Make(AFactoryPlayerController *Controller, UStaticMesh *Mesh) { APlacingActor *Actor = NewObject<APlacingActor>(Controller); Actor->Setup(Controller, Mesh); return Actor; }

	void UpdateConnectionInfo(const FDirectionMap<EConnectionStatus> &Incoming, const FDirectionMap<EConnectionStatus> &Outgoing);

	virtual void Tick(float DeltaSeconds) override;

protected:

	APlacingActor();
	virtual ~APlacingActor() = default;

	virtual void OnClick(FKey ButtonPressed) override;

	// Main mesh component.
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *Mesh;

	// Mapped connection indicator mesh components.
	// Each pair is (INCOMING, OUTGOING)
	//UPROPERTY(EditAnywhere)
	TMap<EDirection, TPair<UStaticMeshComponent*, UStaticMeshComponent*>> ConnectionIndicators;

	// Controller that spawned this placer. May be null if called early.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AFactoryPlayerController *Controller;

private:

	UMaterial *ConnectionInInactiveMat;
	UMaterial *ConnectionInActiveMat;
	UMaterial *ConnectionOutInvalidMat;
	UMaterial *ConnectionOutValidMat;
};