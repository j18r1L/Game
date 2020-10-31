#include "SubMeshComponent.h"

namespace HE
{
    SubMeshComponent::SubMeshComponent(Entity* entityHandle):
        m_Material(nullptr),
        m_ShaderLibrary(nullptr),
        m_ShaderName("undefind")
    {
        m_EntityHandle = entityHandle;
        m_Type = ComponentType::SubMeshComponent;
    }

    SubMeshComponent::SubMeshComponent(Entity* entityHandle, std::shared_ptr<MaterialComponent> materialComponent, std::shared_ptr<ShaderLibrary> shaderLibrary, const std::string& shaderName):
        m_Material(materialComponent),
        m_ShaderLibrary(shaderLibrary),
        m_ShaderName(shaderName)
    {
        m_EntityHandle = entityHandle;
        m_Type = ComponentType::SubMeshComponent;
    }

    void SubMeshComponent::SetAttribute(const std::string& name, std::shared_ptr<VertexArray> vertexArray)
    {
        m_VertexArrays[name] = vertexArray;
    }
    void SubMeshComponent::SetMaterial(const MaterialComponent& materialComponent)
    {
        m_Material = std::make_shared<MaterialComponent>(materialComponent);
    }

    void SubMeshComponent::SetShader(std::shared_ptr<ShaderLibrary> shaderLibrary, const std::string& shaderName)
    {
        m_ShaderLibrary = shaderLibrary;
        m_ShaderName = shaderName;
    }

    const std::unordered_map<std::string, std::shared_ptr<VertexArray>>& SubMeshComponent::GetAttribute() const
    {
        return m_VertexArrays;
    }


    std::shared_ptr<MaterialComponent> SubMeshComponent::GetMaterial() const
    {
        return m_Material;
    }
    std::shared_ptr<Shader> SubMeshComponent::GetShader() const
    {
        return m_ShaderLibrary->Get(m_ShaderName);
    }

    const std::shared_ptr<ShaderLibrary>& SubMeshComponent::GetShaderLibrary() const
    {
        return m_ShaderLibrary;
    }
    const std::string& SubMeshComponent::GetShaderName() const
    {
        return m_ShaderName;
    }

}

