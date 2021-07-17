#include "HudManager.h"

#include "Factory/Character/FactoryPlayerController.h"

#include "Blueprint/UserWidget.h"

UHUDManager::UHUDManager()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FClassFinder<UFactoryHUDWidget> MineHUD;
		ConstructorHelpers::FClassFinder<UFactoryHUDWidget> FactoryHUD;
		ConstructorHelpers::FClassFinder<UFactoryHUDWidget> StoreHUD;
		FConstructorStatics()
			: MineHUD(TEXT("WidgetBlueprint'/Game/Factory/UI/MineHUD'"))
			, FactoryHUD(TEXT("WidgetBlueprint'/Game/Factory/UI/FactoryHUD'"))
			, StoreHUD(TEXT("WidgetBlueprint'/Game/Factory/UI/StoreHUD'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	if (ConstructorStatics.MineHUD.Succeeded())
	{
		this->MineHUD_Class = ConstructorStatics.MineHUD.Class;
	}
	if (ConstructorStatics.FactoryHUD.Succeeded())
	{
		this->FactoryHUD_Class = ConstructorStatics.FactoryHUD.Class;
	}
	if (ConstructorStatics.StoreHUD.Succeeded())
	{
		this->StoreHUD_Class = ConstructorStatics.StoreHUD.Class;
	}
}

UHUDManager::~UHUDManager()
{
	if (CurrentHud)
	{
		//CurrentHud->RemoveFromViewport();
		CurrentHud = nullptr;
	}
	if (CurrentScreen)
	{
		CurrentScreen = nullptr;
	}
}

void UHUDManager::SetGamePlatform(EGamePlatform Platform)
{
	SetScreen(nullptr);

	// TODO transition?
	if (CurrentHud)
	{
		CurrentHud->RemoveFromViewport();
		CurrentHud = nullptr;
	}

	TSubclassOf<class UFactoryHUDWidget> WidgetClass;
	//bool bShowCursor = false;
	switch (Platform)
	{
	case EGamePlatform::STORE:
	default:
		WidgetClass = this->StoreHUD_Class;
		break;
	case EGamePlatform::FACTORY:
		WidgetClass = this->FactoryHUD_Class;
		break;
	case EGamePlatform::MINE:
		WidgetClass = this->MineHUD_Class;
		break;
	}

	CurrentHud = CreateWidget<UFactoryHUDWidget>(GetWorld()->GetFirstPlayerController(), WidgetClass);

	if (CurrentHud)
	{
		if (CurrentCharacter)
		{
			CurrentHud->SetCharacter(CurrentCharacter);
		}

		CurrentHud->AddToViewport(0);
	}
}

bool UHUDManager::IsGamePaused()
{
	return false;
}

void UHUDManager::PauseGame()
{

}

void UHUDManager::SetScreen(UFactoryHUDWidget *Screen)
{
	if (CurrentScreen)
	{
		CurrentScreen->RemoveFromViewport();
		CurrentScreen = nullptr;
	}

	CurrentScreen = Screen;
	if (CurrentScreen)
	{
		if (CurrentCharacter)
		{
			CurrentScreen->SetCharacter(CurrentCharacter);
		}

		CurrentScreen->AddToViewport(0);
	}
}

void UHUDManager::SetCharacter(APlayerCharacter *Character)
{
	CurrentCharacter = Character;
	if (CurrentHud) CurrentHud->SetCharacter(Character);
	if (CurrentScreen) CurrentScreen->SetCharacter(Character);
}