#pragma once

#include "CoreMinimal.h"

#include "ClickablePlatformComponent.h"
#include "Factory/GameEnums.h"
#include "Factory/Logical/Component/LogicalBin.h"
#include "Factory/UI/FactoryInventoryScreen.h"

#include "Bin.generated.h"

UCLASS(BlueprintType)
class ABin : public AClickablePlatformComponent
{
	GENERATED_BODY()
private:

protected:

	// Link to logical bin instnace
	UPROPERTY(VisibleAnywhere)
	ULogicalBin *LogicalBin;

	// If true, bin will spawn static mesh instances of item in the bin
	// indicating the bin's fullness
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowContents = true;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *Content1;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *Content2;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *Content3;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *Content4;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *Content5;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *Content6;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *Content7;

	void UpdateContentDisplay();

	TSubclassOf<class UFactoryInventoryScreen> ScreenClass;

public:

	ABin();

	void SetLogicalBin(ULogicalBin *Bin) { this->LogicalBin = Bin; Refresh(); }
	ULogicalBin *GetLogicalBin() const { return this->LogicalBin; }

	virtual void Refresh() override;

	virtual void ShopTick(EGamePhase Phase) override;

	virtual void SetScreenClass(TSubclassOf<class UFactoryInventoryScreen> ScreenClassIn) { ScreenClass = ScreenClassIn; }

	//virtual void OnClick_Implementation(FKey ButtonPressed) override;
	virtual bool GetClickOptions(ClickOption **DefaultOptOut, TArray<ClickOption> *OptionsOut) override;

};