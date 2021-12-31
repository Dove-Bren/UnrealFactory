#pragma once

// Classes and structs that support special screen set up for showing a set of options
// Most of this is implemented in blueprints.

#include <functional>

#include "CoreMinimal.h"

#include "Factory/UI/FactoryHUDWidget.h"

#include "SelectionPopup.generated.h"

UCLASS(BlueprintType)
class UPopupEntryData : public UObject
{
	GENERATED_BODY()

public:

	UPopupEntryData() {}
	~UPopupEntryData();

	void Init(FName LabelIn, std::function<void()> ActionIn) { this->Label = LabelIn; this->Action = ActionIn; }

	// The text to label this option with
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName Label;

	// Perform the action for this entry
	UFUNCTION(BlueprintCallable)
	void Perform();

protected:

	std::function<void()> Action;

};

UCLASS(BlueprintType, Abstract)
class UPopupMenuWidget : public UFactoryHUDWidget
{
	GENERATED_BODY()

public:

	// Set the options this menu should show
	UFUNCTION(BlueprintImplementableEvent)
	void SetOptions(UPARAM(ref) TArray<UPopupEntryData*> &Options, UPopupEntryData *Default);

protected:

};