#include "ComponentLayout.h"

ULogicalPlatformComponent *UComponentLayout::Remove(int X, int Y)
{
	ULogicalPlatformComponent *Existing = Layout2D<ULogicalPlatformComponent>::Remove(X, Y);
	Components.Remove(Existing);
	return Existing;
}

ULogicalPlatformComponent *UComponentLayout::Insert(int X, int Y, ULogicalPlatformComponent *Element, bool bReplace)
{
	ULogicalPlatformComponent *Existing = Layout2D<ULogicalPlatformComponent>::Insert(X, Y, Element, bReplace);
	if (bReplace || Existing == nullptr)
	{
		if (Existing) // bReplace must be true
		{
			Components.Remove(Existing);
		}

		Components.Add(Element);
	}
	
	return Existing;
}

TArray<ULogicalPlatformComponent*> UComponentLayout::GetComponents()
{
	return Components;
}

ULogicalPlatformComponent *UComponentLayout::GetBP(int CellX, int CellY)
{
	return Get(CellX, CellY);
}

ULogicalPlatformComponent *UComponentLayout::RemoveBP(int CellX, int CellY)
{
	return Remove(CellX, CellY);
}

ULogicalPlatformComponent *UComponentLayout::InsertBP(int CellX, int CellY, ULogicalPlatformComponent *Element, bool bReplace)
{
	return Insert(CellX, CellY, Element, bReplace);
}