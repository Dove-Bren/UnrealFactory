#include "Bin.h"

ABin::ABin() : APlatformComponent()
{
	
}

void ABin::Refresh()
{
	if (LogicalBin)
	{
		this->Mesh->SetStaticMesh(LogicalBin->GetBinMesh());
	}
	else
	{
		Mesh->SetVisibility(false); // Might as well hide it
	}
}