#include "SelectionPopup.h"

UPopupEntryData::~UPopupEntryData()
{
	;
}

void UPopupEntryData::Perform()
{
	if (Action)
	{
		Action();
	}
}
