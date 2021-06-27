#pragma once

#include "CoreMinimal.h"

#include "Engine/GameInstance.h"

#include "Factory/Logical/ItemType.h" // For itemtype registry

#include "FactorySingletons.generated.h"

UCLASS(BlueprintType)
class UFactorySingletons : public UGameInstance
{
	GENERATED_BODY()

public:

	UFactorySingletons();

	static UFactorySingletons *GetInstance(const UObject *WorldContextObject);

	UItemTypeRegistry *GetItemRegistry() { return ItemRegistry; }

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UItemTypeRegistry *ItemRegistry;
};