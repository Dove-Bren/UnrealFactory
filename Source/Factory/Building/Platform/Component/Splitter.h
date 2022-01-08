#pragma once

#include "CoreMinimal.h"

#include "Components/PostProcessComponent.h"

#include "PlatformComponent.h"
#include "Factory/GameEnums.h"
#include "Factory/Logical/Component/LogicalSplitter.h"

#include "Splitter.generated.h"

UCLASS()
class ASplitter : public APlatformComponent
{
	GENERATED_BODY()
private:

protected:

	// Link to logical belt instnace
	UPROPERTY(VisibleAnywhere)
	ULogicalSplitter *LogicalSplitter;

	// Comp for showing items moving on the belt
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *ItemMeshComp;

	// Comp for adjusting blur
	UPROPERTY(VisibleAnywhere)
	UPostProcessComponent *PostProcessComp;

	UMaterial *BeltMat;
	UStaticMesh *SplitterMesh;

public:

	ASplitter();

	void SetLogicalSplitter(ULogicalSplitter *Splitter) { this->LogicalSplitter = Splitter; Refresh(); }
	ULogicalSplitter *GetLogicalSplitter() const { return this->LogicalSplitter; }

	virtual void Refresh() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual ULogicalPlatformComponent *GetLogicalComponent() override { return GetLogicalSplitter(); }

};