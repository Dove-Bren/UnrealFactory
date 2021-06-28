#include "Clickable.h"

#include "Factory/Character/FactoryPlayerController.h"

#include "Components/PrimitiveComponent.h"

AClickableActor::AClickableActor()
{
	OnClicked.AddDynamic(this, &AClickableActor::OnClickHandler);
	OnBeginCursorOver.AddDynamic(this, &AClickableActor::OnHoverStart);
	OnEndCursorOver.AddDynamic(this, &AClickableActor::OnHoverEnd);
}

void AClickableActor::OnClickHandler(AActor *Actor, FKey ButtonPressed)
{
	AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AClickableActor *Clickable = Cast<AClickableActor>(Actor);
	if (Clickable && Controller && !Controller->GetActiveMouseItem())
	{
		Clickable->OnClick(ButtonPressed);
	}
}

void AClickableActor::OnHoverStart(AActor *Actor)
{
	AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AClickableActor *Clickable = Cast<AClickableActor>(Actor);
	if (Clickable && Clickable->bHighlight && Controller && !Controller->GetActiveMouseItem())
	{
		Clickable->ForEachComponent<UPrimitiveComponent>(true, [](UPrimitiveComponent *Component) {
			Component->SetRenderCustomDepth(true);
		});
	}
}

void AClickableActor::OnHoverEnd(AActor *Actor)
{
	AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AClickableActor *Clickable = Cast<AClickableActor>(Actor);
	if (Clickable && Clickable->bHighlight && Controller && !Controller->GetActiveMouseItem())
	{
		Clickable->ForEachComponent<UPrimitiveComponent>(true, [](UPrimitiveComponent *Component) {
			Component->SetRenderCustomDepth(false);
		});
	}
}