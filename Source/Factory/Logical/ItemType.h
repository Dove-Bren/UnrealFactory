#pragma once

// 

#include "CoreMinimal.h"

#include "Factory/GameEnums.h"

#include "Engine/StaticMesh.h"

#include "ItemType.generated.h"

UCLASS(Abstract, Blueprintable)
class UItemType : public UObject
{
	GENERATED_BODY()
protected:

	// Display name of the item type
	UPROPERTY(EditAnywhere)
	FName Name;

	// Description of the item type
	UPROPERTY(EditAnywhere)
	FName Description;

	// Max number of these items that can stack in one slot/Item object
	UPROPERTY(EditAnywhere)
	int32 MaxStackSize;

	// Visual mesh for the item
	UPROPERTY(EditAnywhere)
	UStaticMesh *DisplayMesh;

public:

	UItemType() {};
	virtual ~UItemType() = default;

	UFUNCTION(BlueprintCallable)
	int32 GetMaxStackSize() const { return MaxStackSize; }

	UFUNCTION(BlueprintCallable)
	FName GetName() const { return Name; }

	UFUNCTION(BlueprintCallable)
	FName GetDescription() const { return Description; }

	UFUNCTION(BlueprintCallable)
	UStaticMesh *GetMesh() const { return DisplayMesh; }

};