#pragma once

#include "CoreMinimal.h"

#include "PlatformComponent.h"
#include "Factory/GameEnums.h"
#include "Factory/Logical/Component/LogicalBin.h"

#include "Bin.generated.h"

UCLASS(BlueprintType)
class ABin : public APlatformComponent
{
	GENERATED_BODY()
private:

protected:

	// Link to logical bin instnace
	UPROPERTY(VisibleAnywhere)
	ULogicalBin *LogicalBin;

public:

	ABin();

	void SetLogicalBin(ULogicalBin *Bin) { this->LogicalBin = Bin; Refresh(); }
	ULogicalBin *GetLogicalBin() const { return this->LogicalBin; }

	virtual void Refresh() override;

};