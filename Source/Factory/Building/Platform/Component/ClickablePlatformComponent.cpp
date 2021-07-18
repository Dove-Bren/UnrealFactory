#include "ClickablePlatformComponent.h"

#include "Factory/Character/FactoryPlayerController.h"

#include "Components/PrimitiveComponent.h"

AClickablePlatformComponent::AClickablePlatformComponent(bool bHighlight, EStandardColors HighlightColorIn)
	: APlatformComponent(), bHighlight(bHighlight), HighlightColor(HighlightColorIn)
{
	OnClicked.AddDynamic(this, &AClickablePlatformComponent::OnClickHandler);
	OnBeginCursorOver.AddDynamic(this, &AClickablePlatformComponent::OnHoverStart);
	OnEndCursorOver.AddDynamic(this, &AClickablePlatformComponent::OnHoverEnd);
}

void AClickablePlatformComponent::OnClickHandler(AActor *Actor, FKey ButtonPressed)
{
	AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AClickablePlatformComponent *Clickable = Cast<AClickablePlatformComponent>(Actor);
	if (Clickable && Controller && !Controller->GetActiveMouseItem())
	{
		Clickable->Execute_OnClick(Clickable, ButtonPressed);
	}
}

void AClickablePlatformComponent::OnHoverStart(AActor *Actor)
{
	AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AClickablePlatformComponent *Clickable = Cast<AClickablePlatformComponent>(Actor);
	if (Clickable && Clickable->bHighlight && Controller && !Controller->GetActiveMouseItem())
	{
		const EStandardColors Color = HighlightColor;
		Clickable->ForEachComponent<UPrimitiveComponent>(true, [Color](UPrimitiveComponent *Component) {
			Component->SetRenderCustomDepth(true);
			Component->SetCustomDepthStencilValue((int)GetColorIndex(Color));
		});
	}
}

void AClickablePlatformComponent::OnHoverEnd(AActor *Actor)
{
	AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AClickablePlatformComponent *Clickable = Cast<AClickablePlatformComponent>(Actor);
	if (Clickable && Clickable->bHighlight && Controller && !Controller->GetActiveMouseItem())
	{
		Clickable->ForEachComponent<UPrimitiveComponent>(true, [](UPrimitiveComponent *Component) {
			Component->SetRenderCustomDepth(false);
		});
	}
}