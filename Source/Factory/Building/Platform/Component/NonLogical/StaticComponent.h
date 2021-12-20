#pragma once

// Static platform component that takes up space and not much else!

#include "CoreMinimal.h"

#include "Factory/Building/Platform/Component/ClickablePlatformComponent.h"
#include "Factory/Logical/Component/LogicalStaticComponent.h"

#include "StaticComponent.generated.h"

UCLASS(Blueprintable)
class AStaticComponent : public AClickablePlatformComponent
{
	GENERATED_BODY()

private:

protected:

	// Link to logical compoonent instance
	UPROPERTY(VisibleAnywhere)
	ULogicalStaticComponent *LogicalComponent;

	virtual void OnClick_Implementation(FKey ButtonPressed) override;

public:
	AStaticComponent() : AClickablePlatformComponent(true, EStandardColors::DARK_ORANGE) {}
	virtual ~AStaticComponent() = default;

	void SetLogicalComponent(ULogicalStaticComponent *Component) { this->LogicalComponent = Component; RefreshMesh(); }
	ULogicalStaticComponent *GetLogicalComponent() const { return this->LogicalComponent; }

	void RefreshMesh();

};