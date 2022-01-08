#pragma once

#include "CoreMinimal.h"

#include "Engine/GameInstance.h"

#include "Factory/Logical/Inventory/ItemType.h" // For itemtype registry

#include "FactorySingletons.generated.h"

UCLASS(BlueprintType)
class UFactorySingletons : public UGameInstance
{
	GENERATED_BODY()

public:

	UFactorySingletons();

	UFUNCTION(BlueprintCallable)
	static UFactorySingletons *GetInstance(const UObject *WorldContextObject);

	UItemTypeRegistry *GetItemRegistry() { return ItemRegistry; }

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float PartialTicks = 0.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 TotalTickCount = 0;

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UItemTypeRegistry *ItemRegistry;
};