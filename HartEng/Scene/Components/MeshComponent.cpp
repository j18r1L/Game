#include "MeshComponent.h"

namespace HE
{
    MeshComponent::MeshComponent(Entity* entityHandle)
    {
        m_EntityHandle = entityHandle;
    }

    void MeshComponent::AddSubMesh(SubMeshComponent& subMesh)
    {
        m_SubMeshes.push_back(&subMesh);
    }

    void MeshComponent::SetPath(const std::string& path)
    {
        m_Path = path;
    }

    const std::string& MeshComponent::GetPath()
    {
        return m_Path;
    }

    const std::vector<SubMeshComponent*>& MeshComponent::GetSubMeshes() const
    {
        return m_SubMeshes;
    }
}

