#include "StaticComponent.h"

#include "Factory/Character/FactoryPlayerController.h"
#include "Factory/Character/PlayerCharacter.h"

void AStaticComponent::RefreshMesh()
{
	if (LogicalComponent)
	{
		UStaticMesh *MeshAsset = LogicalComponent->GetStaticMesh();
		if (MeshAsset)
		{
			this->Mesh->SetStaticMesh(MeshAsset);
			// Also take and set a material?
		}
	}
}