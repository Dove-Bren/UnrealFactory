#pragma once

// A single component that exists on a platform.
// This is the base class, and includes placables as well as obstacles, etc.
// Basically anything that interacts with other components should be a component.

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

#include "Factory/GameEnums.h"
#include "Factory/Clickable.h"

#include "PlatformComponent.generated.h"

typedef class UPlatform UPlatform;

typedef class ULogicalPlatformComponent ULogicalPlatformComponent;
typedef struct ClickOption ClickOption;

UCLASS(Blueprintable, Abstract)
class APlatformComponent : public AActor, public IClickable
{
	GENERATED_BODY()
public:

	APlatformComponent();
	virtual ~APlatformComponent() = default;

	// TODO remove these as they are part of logical

	// Init the component as part of the provided platform
	UFUNCTION(BlueprintCallable)
	virtual void RegisterPlatform(UPlatform *Platform);

	// Detatch the component from the platform
	UFUNCTION(BlueprintCallable)
	virtual void RemoveFromPlatform();

	// Transition this component to the provided daily phase
	UFUNCTION(BlueprintCallable)
	virtual void StartPhase(EGamePhase Phase);

	// Check whether this component is active in the provided phase
	UFUNCTION(BlueprintCallable)
	virtual bool IsActiveDuring(EGamePhase Phase);

	// Perform regular shop tick functions for the provided phase
	UFUNCTION(BlueprintCallable)
	virtual void ShopTick(EGamePhase Phase);

	virtual void Refresh() {}

	virtual void OnClick_Implementation(FKey ButtonPressed) override;

	virtual void Tick(float DeltaSeconds) override;

	UPlatform *GetPlatformRef() { return ParentPlatform; }

protected:

	// Base mesh component
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *Mesh;

	// Owning Platform
	UPROPERTY(EditAnywhere)
	UPlatform *ParentPlatform;

	virtual ULogicalPlatformComponent *GetLogicalComponent() { return nullptr; }

	virtual bool GetClickOptions(ClickOption **DefaultOptOut, TArray<ClickOption> *OptionsOut);
	virtual TSubclassOf<class UPopupMenuWidget> GetMenuWidgetClass();

	virtual bool ShouldHighlight_Implementation() override;
	virtual EStandardColors GetHighlightColor_Implementation() override;

private:

	UFUNCTION()
	void OnClickHandler(AActor *Actor, FKey ButtonPressed);

	UFUNCTION()
	void OnHoverStart(AActor *Actor);

	UFUNCTION()
	void OnHoverEnd(AActor *Actor);

};