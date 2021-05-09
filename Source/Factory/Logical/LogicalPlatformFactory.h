#pragma once

// Specialized platform for managing factory layers

#include "CoreMinimal.h"

#include "LogicalPlatform.h"

#include "LogicalPlatformFactory.generated.h"

#define DEFAULT_WIDTH MIN_PLATFORM_WIDTH
#define DEFAULT_HEIGHT MIN_PLATFORM_HEIGHT

UCLASS()
class ULogicalPlatformFactory : public ULogicalPlatform
{
	GENERATED_BODY()
private:

protected:

public:
	ULogicalPlatformFactory();
	~ULogicalPlatformFactory() = default;

};