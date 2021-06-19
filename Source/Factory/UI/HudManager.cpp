#include "HudManager.h"

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
	if (CurrentWidget)
	{
		//CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}
}

void UHUDManager::SetGamePlatform(EGamePlatform Platform)
{
	// TODO transition?
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	TSubclassOf<class UFactoryHUDWidget> WidgetClass;
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

	CurrentWidget = CreateWidget<UFactoryHUDWidget>(GetWorld()->GetFirstPlayerController(), WidgetClass);

	if (CurrentWidget)
	{
		if (CurrentCharacter)
		{
			CurrentWidget->SetCharacter(CurrentCharacter);
		}

		CurrentWidget->AddToViewport(0);
	}
}

bool UHUDManager::IsGamePaused()
{
	return false;
}

void UHUDManager::PauseGame()
{

}