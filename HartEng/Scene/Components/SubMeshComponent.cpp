/*
#include "SubMeshComponent.h"

namespace HE
{
    SubMeshComponent::SubMeshComponent(Entity* entityHandle):
        m_Material(nullptr)
    {
        m_EntityHandle = entityHandle;
    }

    void SubMeshComponent::SetAttribute(std::shared_ptr<VertexArray> vertexArray)
    {
        m_VertexArray = vertexArray;
    }

    void SubMeshComponent::SetName(const std::string& name)
    {
        m_Name = name;
    }

    void SubMeshComponent::SetMaterial(const MaterialComponent& materialComponent)
    {
        m_Material = std::make_shared<MaterialComponent>(materialComponent);
    }

    void SubMeshComponent::SetMaterial(MaterialComponent* materialComponent)
    {
        m_Material.reset(materialComponent);
    }

    const std::shared_ptr<VertexArray>& SubMeshComponent::GetAttribute() const
    {
        return m_VertexArray;
    }

    const std::string& SubMeshComponent::GetName() const
    {
        return m_Name;
    }

    std::shared_ptr<MaterialComponent> SubMeshComponent::GetMaterial() const
    {
        return m_Material;
    }


}

*/