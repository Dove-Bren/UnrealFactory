#pragma once

// 

#include "EngineMinimal.h"

#include "Factory/GameEnums.h"

#include "Clickable.generated.h"

UINTERFACE()
class UClickable : public UInterface
{
	GENERATED_BODY()
};

class IClickable
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnClick(FKey ButtonPressed);

protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ShouldHighlight();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EStandardColors GetHighlightColor();
};