#include "MeshComponent.h"

namespace HE
{
    MeshComponent::MeshComponent(Entity* entityHandle)
    {
        m_EntityHandle = entityHandle;
        m_Type = ComponentType::MeshComponent;
    }

    void MeshComponent::AddSubMesh(SubMeshComponent& subMesh)
    {
        m_SubMeshes.push_back(&subMesh);
    }

    const std::vector<SubMeshComponent*>& MeshComponent::GetSubMeshes() const
    {
        return m_SubMeshes;
    }

    /*
    std::vector<SubMeshComponent*>& MeshComponent::GetSubMeshes()
    {
        return m_SubMeshes;
    }
    */
}

