#include "MeshComponent.h"

namespace HE
{
    MeshComponent::MeshComponent(Entity* entityHandle)
    {
        m_EntityHandle = entityHandle;
    }

    void MeshComponent::Copy(Component* other_base)
    {
        MeshComponent* other = dynamic_cast<MeshComponent*>(other_base);
        m_Mesh = other->m_Mesh;
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

