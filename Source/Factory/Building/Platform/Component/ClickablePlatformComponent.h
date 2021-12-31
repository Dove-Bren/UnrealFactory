//#pragma once
//
//// Convenience subclass of IBlockable and PlatformComponent
//
//#include <functional>
//
//#include "EngineMinimal.h"
//
//#include "GameFramework/Actor.h"
//
//#include "Factory/GameEnums.h"
//#include "Factory/Clickable.h"
//#include "Factory/Building/Platform/Component/PlatformComponent.h"
//
//#include "ClickablePlatformComponent.generated.h"
//
//typedef struct ClickOption ClickOption;
//
//UCLASS(Blueprintable, Abstract)
//class AClickablePlatformComponent : public APlatformComponent, public IClickable
//{
//	GENERATED_BODY()
//public:
//
//	AClickablePlatformComponent(bool bHighlight, EStandardColors HighlightColorIn = EStandardColors::ORANGE);
//	AClickablePlatformComponent() : AClickablePlatformComponent(true) {};
//	
//	virtual ~AClickablePlatformComponent() = default;
//
//	virtual void OnClick_Implementation(FKey ButtonPressed) override;
//
//	virtual void Tick(float DeltaSeconds) override;
//
//protected:
//
//	bool bHighlight = true;
//	EStandardColors HighlightColor = EStandardColors::ORANGE;
//	TSubclassOf<class UPopupMenuWidget> PopupWidgetClass;
//
//	virtual bool ShouldHighlight_Implementation() override;
//	virtual EStandardColors GetHighlightColor_Implementation() override;
//
//	virtual bool GetClickOptions(ClickOption **DefaultOptOut, TArray<ClickOption> *OptionsOut);
//
//private:
//};
//
//typedef struct ClickOption {
//	FName Name;
//	std::function<void()> OnClick;
//
//	ClickOption(FName Name, std::function<void()> OnClick) : Name(Name), OnClick(OnClick) {}
//} ClickOption;
