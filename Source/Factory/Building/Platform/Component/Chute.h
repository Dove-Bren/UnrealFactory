#pragma once

#include "CoreMinimal.h"

#include "PlatformComponent.h"
#include "Factory/GameEnums.h"
#include "Factory/Logical/Component/LogicalResourceChute.h"

#include "Chute.generated.h"

UCLASS(BlueprintType)
class AChute : public APlatformComponent
{
	GENERATED_BODY()
private:

protected:

	// Link to logical bin instnace
	UPROPERTY(VisibleAnywhere)
	ULogicalResourceChute *LogicalChute;

public:

	AChute();

	void SetLogicalChute(ULogicalResourceChute *Chute) { this->LogicalChute = Chute; Refresh(); }
	ULogicalResourceChute *GetLogicalChute() const { return this->LogicalChute; }

	virtual void Refresh() override;

};