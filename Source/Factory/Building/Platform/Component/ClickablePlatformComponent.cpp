//#include "ClickablePlatformComponent.h"
//
//#include "Factory/Character/FactoryPlayerController.h"
//#include "Factory/Character/PlayerCharacter.h"
//#include "Factory/UI/Popup/SelectionPopup.h"
//
//#include "Components/PrimitiveComponent.h"
//
//AClickablePlatformComponent::AClickablePlatformComponent(bool bHighlight, EStandardColors HighlightColorIn)
//	: APlatformComponent(), bHighlight(bHighlight), HighlightColor(HighlightColorIn)
//{
//	struct FConstructorStatics
//	{
//		ConstructorHelpers::FClassFinder<UPopupMenuWidget> PopupWidget;
//		FConstructorStatics()
//			: PopupWidget(TEXT("WidgetBlueprint'/Game/Factory/UI/Screens/PopupMenu/PopupSelectionMenu'"))
//		{
//		}
//	};
//	static FConstructorStatics ConstructorStatics;
//
//	if (ConstructorStatics.PopupWidget.Succeeded())
//	{
//		this->PopupWidgetClass = ConstructorStatics.PopupWidget.Class;
//	}
//
//	OnClicked.AddDynamic(this, &AClickablePlatformComponent::OnClickHandler);
//	OnBeginCursorOver.AddDynamic(this, &AClickablePlatformComponent::OnHoverStart);
//	OnEndCursorOver.AddDynamic(this, &AClickablePlatformComponent::OnHoverEnd);
//	PrimaryActorTick.bCanEverTick = true;
//}
//
//void AClickablePlatformComponent::OnClickHandler(AActor *Actor, FKey ButtonPressed)
//{
//	AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
//	AClickablePlatformComponent *Clickable = Cast<AClickablePlatformComponent>(Actor);
//	if (Clickable && Controller && !Controller->GetActiveMouseItem())
//	{
//		Clickable->Execute_OnClick(Clickable, ButtonPressed);
//	}
//}
//
//void AClickablePlatformComponent::OnHoverStart(AActor *Actor)
//{
//	AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
//	AClickablePlatformComponent *Clickable = Cast<AClickablePlatformComponent>(Actor);
//	if (Clickable && Execute_ShouldHighlight(Clickable) && Controller && !Controller->GetActiveMouseItem())
//	{
//		const EStandardColors Color = Execute_GetHighlightColor(Clickable);
//		Clickable->ForEachComponent<UPrimitiveComponent>(true, [Color](UPrimitiveComponent *Component) {
//			Component->SetRenderCustomDepth(true);
//			Component->SetCustomDepthStencilValue((int)GetColorIndex(Color));
//		});
//	}
//}
//
//void AClickablePlatformComponent::OnHoverEnd(AActor *Actor)
//{
//	AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
//	AClickablePlatformComponent *Clickable = Cast<AClickablePlatformComponent>(Actor);
//	if (Clickable && Execute_ShouldHighlight(Clickable) && Controller && !Controller->GetActiveMouseItem())
//	{
//		Clickable->ForEachComponent<UPrimitiveComponent>(true, [](UPrimitiveComponent *Component) {
//			Component->SetRenderCustomDepth(false);
//		});
//	}
//}
//
//bool AClickablePlatformComponent::ShouldHighlight_Implementation()
//{
//	//if (bHighlight)
//	//{
//	//	// Check reach
//	//	AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(UGameplayStatics::GetPlayerController(this->GetWorld(), 0));
//	//	APlayerCharacter *Character = (!!Controller ? Cast<APlayerCharacter>(Controller->GetCharacter()) : nullptr);
//	//	if (!Character || Character->GetDistanceTo(this) <= Character->GetMaxReach())
//	//	{
//	//		return true;
//	//	}
//	//}
//
//	//return false;
//	return bHighlight;
//}
//
//EStandardColors AClickablePlatformComponent::GetHighlightColor_Implementation()
//{
//	// Use hardcoded unreachable color if too far. Otherwise, return color.
//	AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(UGameplayStatics::GetPlayerController(this->GetWorld(), 0));
//	APlayerCharacter *Character = (!!Controller ? Cast<APlayerCharacter>(Controller->GetCharacter()) : nullptr);
//	if (!Character || Character->GetDistanceTo(this) <= Character->GetMaxReach())
//	{
//		return HighlightColor;
//	}
//	
//	return EStandardColors::DARK_GRAY;
//}
//
//void AClickablePlatformComponent::OnClick_Implementation(FKey ButtonPressed)
//{
//	// Get options from component
//	TArray<ClickOption> Options;
//	ClickOption *Default = nullptr;
//	if (this->GetClickOptions(&Default, &Options) && Options.Num() > 0)
//	{
//		if (!Default)
//		{
//			Default = &Options[0];
//		}
//
//		if (ButtonPressed == EKeys::RightMouseButton)
//		{
//			// Perform default
//			Default->OnClick();
//		}
//		else
//		{
//			// Display menu
//			AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(UGameplayStatics::GetPlayerController(this->GetWorld(), 0));
//			if (Controller)
//			{
//				UPopupMenuWidget *Screen = CreateWidget<UPopupMenuWidget>(this->GetWorld()->GetFirstPlayerController(), this->PopupWidgetClass);
//
//				if (Screen)
//				{
//					// Need to convert from our ClickOption types to UPopupEntryDatas
//					TArray<UPopupEntryData*> MenuOptions;
//					UPopupEntryData *MenuDefault = nullptr;
//
//					for (ClickOption& Option : Options)
//					{
//						UPopupEntryData *Data = NewObject<UPopupEntryData>(Screen);
//						Data->Init(Option.Name, Option.OnClick);
//						MenuOptions.Add(Data);
//						if (Option.Name == Default->Name) {
//							MenuDefault = Data;
//						}
//					}
//
//					if (!MenuDefault)
//					{
//						MenuDefault = NewObject<UPopupEntryData>(Screen);
//						MenuDefault->Init(Default->Name, Default->OnClick);
//					}
//
//					Screen->SetOptions(MenuOptions, MenuDefault);
//					Controller->OpenScreenAt(Screen, this->GetActorLocation());
//				}
//			}
//		}
//	}
//	// Else returned false; no menu!
//}
//
//// Default version cause can't have pure virtual methods in UE4
//bool AClickablePlatformComponent::GetClickOptions(ClickOption **DefaultOptOut, TArray<ClickOption> *OptionsOut)
//{
//	return false;
//}
//
//void AClickablePlatformComponent::Tick(float DeltaSeconds)
//{
//	Super::Tick(DeltaSeconds);
//
//	const EStandardColors Color = Execute_GetHighlightColor(this);
//	this->ForEachComponent<UPrimitiveComponent>(true, [Color](UPrimitiveComponent *Component) {
//		Component->SetCustomDepthStencilValue((int)GetColorIndex(Color));
//	});
//}