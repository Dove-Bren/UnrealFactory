#pragma once

// 

#include "EngineMinimal.h"

#include "GameFramework/Actor.h"

#include "Factory/GameEnums.h"

#include "Clickable.generated.h"

UCLASS(Blueprintable, Abstract)
class AClickableActor : public AActor
{
	GENERATED_BODY()
public:

	AClickableActor(bool bHighlight, EStandardColors HighlightColorIn = EStandardColors::ORANGE);
	AClickableActor() : AClickableActor(true) {};
	
	virtual ~AClickableActor() = default;

protected:

	bool bHighlight = true;
	EStandardColors HighlightColor = EStandardColors::ORANGE;

	virtual void OnClick(FKey ButtonPressed) {};

private:

	UFUNCTION()
	void OnClickHandler(AActor *Actor, FKey ButtonPressed);

	UFUNCTION()
	void OnHoverStart(AActor *Actor);

	UFUNCTION()
	void OnHoverEnd(AActor *Actor);

};
