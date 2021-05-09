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

	// Link to logical belt instnace
	UPROPERTY(VisibleAnywhere)
	ULogicalBelt *LogicalBelt;

protected:

public:

	ABelt();

	void SetLogicalBelt(ULogicalBelt *Belt) { this->LogicalBelt = Belt; RefreshMesh(); }

	void RefreshMesh();

};