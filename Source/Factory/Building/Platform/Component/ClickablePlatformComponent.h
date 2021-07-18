#pragma once

// Convenience subclass of IBlockable and PlatformComponent

#include "EngineMinimal.h"

#include "GameFramework/Actor.h"

#include "Factory/GameEnums.h"
#include "Factory/Clickable.h"
#include "Factory/Building/Platform/Component/PlatformComponent.h"

#include "ClickablePlatformComponent.generated.h"

UCLASS(Blueprintable, Abstract)
class AClickablePlatformComponent : public APlatformComponent, public IClickable
{
	GENERATED_BODY()
public:

	AClickablePlatformComponent(bool bHighlight, EStandardColors HighlightColorIn = EStandardColors::ORANGE);
	AClickablePlatformComponent() : AClickablePlatformComponent(true) {};
	
	virtual ~AClickablePlatformComponent() = default;

	//virtual void OnClick_Implementation(FKey ButtonPressed) override {};

protected:

	bool bHighlight = true;
	EStandardColors HighlightColor = EStandardColors::ORANGE;

	virtual bool ShouldHighlight_Implementation() override { return bHighlight; };
	virtual EStandardColors GetHighlightColor_Implementation() override { return HighlightColor; };

private:

	UFUNCTION()
	void OnClickHandler(AActor *Actor, FKey ButtonPressed);

	UFUNCTION()
	void OnHoverStart(AActor *Actor);

	UFUNCTION()
	void OnHoverEnd(AActor *Actor);

};
