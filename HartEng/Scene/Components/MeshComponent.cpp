#include "MeshComponent.h"

namespace HE
{
    MeshComponent::MeshComponent(Entity* entityHandle)
    {
        m_EntityHandle = entityHandle;
    }

    void MeshComponent::SetMesh(const std::shared_ptr<Mesh>& mesh)
    {
        m_Mesh = mesh;
    }

    std::shared_ptr<Mesh> MeshComponent::GetMesh()
    {
        return m_Mesh;
    }



}

