#pragma once

// Base actor implementation of IClickable

#include "EngineMinimal.h"

#include "Factory/GameEnums.h"
#include "Clickable.h"

#include "ClickableActor.generated.h"

UCLASS(Blueprintable, Abstract)
class AClickableActor : public AActor, public IClickable
{
	GENERATED_BODY()
public:

	AClickableActor(bool bHighlight, EStandardColors HighlightColorIn = EStandardColors::ORANGE);
	AClickableActor() : AClickableActor(true) {};

	virtual ~AClickableActor() = default;

protected:

	bool bHighlight = true;
	EStandardColors HighlightColor = EStandardColors::ORANGE;

	bool ShouldHighlight_Implementation() { return bHighlight; }

	EStandardColors GetHighlightColor_Implementation() { return HighlightColor; }

private:

	UFUNCTION()
	void OnClickHandler(AActor *Actor, FKey ButtonPressed);

	UFUNCTION()
	void OnHoverStart(AActor *Actor);

	UFUNCTION()
	void OnHoverEnd(AActor *Actor);

};
