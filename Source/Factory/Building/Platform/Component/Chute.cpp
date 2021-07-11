#include "Chute.h"

AChute::AChute() : APlatformComponent()
{
	
}

void AChute::Refresh()
{
	if (LogicalChute)
	{
		this->Mesh->SetStaticMesh(LogicalChute->GetChuteMesh());
	}
	else
	{
		Mesh->SetVisibility(false); // Might as well hide it
	}
}