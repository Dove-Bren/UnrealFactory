#pragma once

// Slightly specialized widget for regular gameplay HUD elements

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

//#include "Factory/Character/PlayerCharacter.h"
//#include "Factory/Building/Shop.h"

#include "FactoryHUDWidget.generated.h"

typedef class APlayerCharacter APlayerCharacter;

typedef class AShop AShop;

UCLASS()
class UFactoryHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// Set the character this hud will represent
	UFUNCTION(BlueprintCallable)
	void SetCharacter(APlayerCharacter *CharacterIn) { Character = CharacterIn; }

	// Get the current character if one is set
	UFUNCTION(BlueprintCallable)
	APlayerCharacter *GetCharacter() { return Character; }

	// Get the shop associated with the current character, if any.
	// Just a wrapper for Character->GetShop().
	UFUNCTION(BlueprintCallable)
	AShop *GetCurrentShop();

protected:

	// The Character this HUD is for
	UPROPERTY(VisibleAnywhere)
	APlayerCharacter *Character;

};