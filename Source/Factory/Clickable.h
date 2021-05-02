#pragma once

// 

#include "EngineMinimal.h"

#include "GameFramework/Actor.h"

#include "Clickable.generated.h"

UCLASS(Blueprintable)
class AClickableActor : public AActor
{
	GENERATED_BODY()
public:

	AClickableActor(bool bHighlight) : bHighlight(bHighlight) {}
	AClickableActor();
	
	virtual ~AClickableActor() = default;

protected:

	virtual void OnClick(FKey ButtonPressed) {};

private:
	bool bHighlight = true;

	UFUNCTION()
	void OnClickHandler(AActor *Actor, FKey ButtonPressed);

	UFUNCTION()
	void OnHoverStart(AActor *Actor);

	UFUNCTION()
	void OnHoverEnd(AActor *Actor);

};
