#include "ComponentLayout.h"

APlatformComponent *UComponentLayout::Remove(int X, int Y)
{
	APlatformComponent *Existing = Layout2D<APlatformComponent>::Remove(X, Y);
	Components.Remove(Existing);
	return Existing;
}

APlatformComponent *UComponentLayout::Insert(int X, int Y, APlatformComponent *Element, bool bReplace)
{
	APlatformComponent *Existing = Layout2D<APlatformComponent>::Insert(X, Y, Element, bReplace);
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

TArray<APlatformComponent*> UComponentLayout::GetComponents()
{
	return Components;
}

APlatformComponent *UComponentLayout::GetBP(int CellX, int CellY)
{
	return Get(CellX, CellY);
}

APlatformComponent *UComponentLayout::RemoveBP(int CellX, int CellY)
{
	return Remove(CellX, CellY);
}

APlatformComponent *UComponentLayout::InsertBP(int CellX, int CellY, APlatformComponent *Element, bool bReplace)
{
	return Insert(CellX, CellY, Element, bReplace);
}