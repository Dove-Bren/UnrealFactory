#include "PlatformComponent.h"

#include "Factory/Character/FactoryPlayerController.h"
#include "Factory/Character/PlayerCharacter.h"
#include "Factory/UI/Popup/SelectionPopup.h"
#include "Factory/Logical/Component/LogicalPlatformComponent.h"

APlatformComponent::APlatformComponent()
{
	int i = 4;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	this->RootComponent = Mesh;

	OnClicked.AddDynamic(this, &APlatformComponent::OnClickHandler);
	OnBeginCursorOver.AddDynamic(this, &APlatformComponent::OnHoverStart);
	OnEndCursorOver.AddDynamic(this, &APlatformComponent::OnHoverEnd);
	PrimaryActorTick.bCanEverTick = true;
}

void APlatformComponent::RegisterPlatform(UPlatform * Platform)
{
	this->ParentPlatform = Platform;
	Platform->AddComponent(this);
}

void APlatformComponent::RemoveFromPlatform()
{
	this->ParentPlatform->RemoveComponent(this);
	this->ParentPlatform = nullptr;
}

void APlatformComponent::StartPhase(EGamePhase Phase)
{
	;
}

bool APlatformComponent::IsActiveDuring(EGamePhase Phase)
{
	return true;
}

void APlatformComponent::ShopTick(EGamePhase Phase)
{
	;
}

void APlatformComponent::OnClickHandler(AActor *Actor, FKey ButtonPressed)
{
	this->Execute_OnClick(this, ButtonPressed);
}

void APlatformComponent::OnHoverStart(AActor *Actor)
{
	if (Execute_ShouldHighlight(this))
	{
		const EStandardColors Color = Execute_GetHighlightColor(this);
		this->ForEachComponent<UPrimitiveComponent>(true, [Color](UPrimitiveComponent *Component) {
			Component->SetRenderCustomDepth(true);
			Component->SetCustomDepthStencilValue((int)GetColorIndex(Color));
		});
	}
}

void APlatformComponent::OnHoverEnd(AActor *Actor)
{
	if (Execute_ShouldHighlight(this))
	{
		this->ForEachComponent<UPrimitiveComponent>(true, [](UPrimitiveComponent *Component) {
			Component->SetRenderCustomDepth(false);
		});
	}
}

bool APlatformComponent::ShouldHighlight_Implementation()
{
	AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	APlayerCharacter *Character = (!!Controller ? Cast<APlayerCharacter>(Controller->GetCharacter()) : nullptr);
	ULogicalPlatformComponent *LogicalComponent = this->GetLogicalComponent();

	if (LogicalComponent)
	{
		float Distance = (Character ? Character->GetDistanceTo(this) : 0);
		return LogicalComponent->ShouldHighlight(Character, Distance);
	}
	
	return false;
}

EStandardColors APlatformComponent::GetHighlightColor_Implementation()
{
	AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	APlayerCharacter *Character = (!!Controller ? Cast<APlayerCharacter>(Controller->GetCharacter()) : nullptr);
	ULogicalPlatformComponent *LogicalComponent = this->GetLogicalComponent();

	if (LogicalComponent)
	{
		float Distance = (Character ? Character->GetDistanceTo(this) : 0);
		return LogicalComponent->GetHighlightColor(Character, Distance);
	}

	return EStandardColors::ORANGE;
}

bool APlatformComponent::GetClickOptions(ClickOption **DefaultOptOut, TArray<ClickOption> *OptionsOut)
{
	AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	APlayerCharacter *Character = (!!Controller ? Cast<APlayerCharacter>(Controller->GetCharacter()) : nullptr);
	ULogicalPlatformComponent *LogicalComponent = this->GetLogicalComponent();

	if (LogicalComponent)
	{
		// Get options from component
		float Distance = (Character ? Character->GetDistanceTo(this) : 0);
		return LogicalComponent->GetClickOptions(Character, Distance, this->GetActorLocation(), DefaultOptOut, OptionsOut);
	}

	return false;
}

TSubclassOf<class UPopupMenuWidget> APlatformComponent::GetMenuWidgetClass()
{
	ULogicalPlatformComponent *LogicalComponent = this->GetLogicalComponent();

	if (LogicalComponent)
	{
		return LogicalComponent->GetMenuWidgetClass();
	}

	return nullptr;
}

void APlatformComponent::OnClick_Implementation(FKey ButtonPressed)
{
	// Get options from component
	TArray<ClickOption> Options;
	ClickOption *Default = nullptr;

	if (GetClickOptions(&Default, &Options))
	{
		if (Options.Num() > 0)
		{
			AFactoryPlayerController *Controller = Cast<AFactoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			APlayerCharacter *Character = (!!Controller ? Cast<APlayerCharacter>(Controller->GetCharacter()) : nullptr);
			float Distance = (Character ? Character->GetDistanceTo(this) : 0);

			if (!Default)
			{
				Default = &Options[0];
			}

			if (ButtonPressed == EKeys::RightMouseButton)
			{
				// Perform default & dismiss any popup that max be up
				Controller->GetHudManager()->SetPopup(nullptr);
				Default->OnClick();
			}
			else
			{
				// Display menu
				if (Controller)
				{
					UPopupMenuWidget *Screen = CreateWidget<UPopupMenuWidget>(Controller, GetMenuWidgetClass());

					if (Screen)
					{
						// Need to convert from our ClickOption types to UPopupEntryDatas
						TArray<UPopupEntryData*> MenuOptions;
						UPopupEntryData *MenuDefault = nullptr;

						for (ClickOption& Option : Options)
						{
							UPopupEntryData *Data = NewObject<UPopupEntryData>(Screen);
							Data->Init(Option.Name, Option.OnClick);
							MenuOptions.Add(Data);
							if (Option.Name == Default->Name) {
								MenuDefault = Data;
							}
						}

						if (!MenuDefault)
						{
							MenuDefault = NewObject<UPopupEntryData>(Screen);
							MenuDefault->Init(Default->Name, Default->OnClick);
						}

						Screen->SetOptions(MenuOptions, MenuDefault);
						Controller->GetHudManager()->SetPopup(Screen);
						//Controller->OpenScreenAt(Screen, this->GetActorLocation());
					}
				}
			}
		}
	}
	// Else returned false; no menu!
}

void APlatformComponent::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const EStandardColors Color = Execute_GetHighlightColor(this);
	this->ForEachComponent<UPrimitiveComponent>(true, [Color](UPrimitiveComponent *Component) {
		Component->SetCustomDepthStencilValue((int)GetColorIndex(Color));
	});
}