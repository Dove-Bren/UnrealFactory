#pragma once

#include "CoreMinimal.h"

#include "PlatformComponent.h"
#include "Factory/GameEnums.h"
#include "Factory/Logical/Component/LogicalBelt.h"

#include "Belt.generated.h"

UCLASS()
class ABelt : public APlatformComponent
{
	GENERATED_BODY()
private:

protected:

	// Link to logical belt instnace
	UPROPERTY(VisibleAnywhere)
	ULogicalBelt *LogicalBelt;

	// Comp for showing items moving on the belt
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *ItemMeshComp;

public:

	ABelt();

	void SetLogicalBelt(ULogicalBelt *Belt) { this->LogicalBelt = Belt; Refresh(); }
	ULogicalBelt *GetLogicalBelt() const { return this->LogicalBelt; }

	virtual void Refresh() override;

	virtual void Tick(float DeltaSeconds) override;

};