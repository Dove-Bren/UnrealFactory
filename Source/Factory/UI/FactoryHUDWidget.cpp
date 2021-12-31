#include "FactoryHUDWidget.h"

#include "Factory/Character/PlayerCharacter.h"
#include "Factory/Building/Shop.h"

AShop *UFactoryHUDWidget::GetCurrentShop()
{
	return Character ? Character->GetShop() : nullptr;
}