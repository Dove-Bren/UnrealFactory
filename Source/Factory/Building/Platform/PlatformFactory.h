#pragma once

// Specialized platform for managing factory layers

#include "CoreMinimal.h"

#include "Platform.h"

#include "PlatformFactory.generated.h"

UCLASS()
class UPlatformFactory : public UPlatform
{
	GENERATED_BODY()
private:

protected:

public:
	UPlatformFactory();
	~UPlatformFactory() = default;

};