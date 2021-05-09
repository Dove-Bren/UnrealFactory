#pragma once

// Specialized platform for managing shop layers

#include "CoreMinimal.h"

#include "LogicalPlatform.h"

#include "LogicalPlatformStore.generated.h"

#define DEFAULT_WIDTH MIN_PLATFORM_WIDTH
#define DEFAULT_HEIGHT MIN_PLATFORM_HEIGHT

UCLASS()
class ULogicalPlatformStore : public ULogicalPlatform
{
	GENERATED_BODY()
private:



protected:


public:
	ULogicalPlatformStore();
	~ULogicalPlatformStore() = default;


};