#pragma once

#include "CoreMinimal.h"

#include "Components/PostProcessComponent.h"

#include "PlatformComponent.h"
#include "Factory/GameEnums.h"
#include "Factory/Logical/Component/LogicalMerger.h"

#include "Merger.generated.h"

UCLASS()
class AMerger : public APlatformComponent
{
	GENERATED_BODY()
private:

protected:

	// Link to logical belt instnace
	UPROPERTY(VisibleAnywhere)
	ULogicalMerger *LogicalMerger;

	// Comp for showing items moving on the belt
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *ItemMeshComp;

	// Comp for adjusting blur
	UPROPERTY(VisibleAnywhere)
	UPostProcessComponent *PostProcessComp;

	UMaterial *BeltMat;
	UStaticMesh *MergerMesh;

public:

	AMerger();

	void SetLogicalMerger(ULogicalMerger *Merger) { this->LogicalMerger = Merger; Refresh(); }
	ULogicalMerger *GetLogicalMerger() const { return this->LogicalMerger; }

	virtual void Refresh() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual ULogicalPlatformComponent *GetLogicalComponent() override { return GetLogicalMerger(); }

};