#include "Clickable.h"

#include "Components/PrimitiveComponent.h"

AClickableActor::AClickableActor()
{
	OnClicked.AddDynamic(this, &AClickableActor::OnClickHandler);
	OnBeginCursorOver.AddDynamic(this, &AClickableActor::OnHoverStart);
	OnEndCursorOver.AddDynamic(this, &AClickableActor::OnHoverEnd);
}

void AClickableActor::OnClickHandler(AActor *Actor, FKey ButtonPressed)
{
	AClickableActor *Clickable = Cast<AClickableActor>(Actor);
	if (Clickable)
	{
		Clickable->OnClick(ButtonPressed);
	}
}

void AClickableActor::OnHoverStart(AActor *Actor)
{
	AClickableActor *Clickable = Cast<AClickableActor>(Actor);
	if (Clickable && Clickable->bHighlight)
	{
		Clickable->ForEachComponent<UPrimitiveComponent>(true, [](UPrimitiveComponent *Component) {
			Component->SetRenderCustomDepth(true);
		});
	}
}

void AClickableActor::OnHoverEnd(AActor *Actor)
{
	AClickableActor *Clickable = Cast<AClickableActor>(Actor);
	if (Clickable && Clickable->bHighlight)
	{
		Clickable->ForEachComponent<UPrimitiveComponent>(true, [](UPrimitiveComponent *Component) {
			Component->SetRenderCustomDepth(false);
		});
	}
}