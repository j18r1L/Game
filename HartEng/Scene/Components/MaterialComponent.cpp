#include "MaterialComponent.h"

namespace HE
{

    MaterialComponent::MaterialComponent():
        m_Textures(),
        m_Emissive(0.0f),
        m_ShaderLibrary(nullptr),
        m_ShaderName("undefind")
    {
        m_EntityHandle = nullptr;
        m_Type = ComponentType::MaterialComponent;
    }

    MaterialComponent::MaterialComponent(Entity* entityHandle):
        m_Textures(),
        m_Emissive(0.0f),
        m_ShaderLibrary(nullptr),
        m_ShaderName("undefind")
    {
        m_EntityHandle = entityHandle;
        m_Type = ComponentType::MaterialComponent;
    }

    void MaterialComponent::AddTexture(const std::string& name, Texture2DComponent* texture2DComponent)
    {
        std::shared_ptr<Texture2DComponent> texture2DComponent_ptr(texture2DComponent);
        m_Textures[name] = texture2DComponent_ptr;
    }

    void MaterialComponent::AddTexture(const std::string& name, std::shared_ptr<Texture2DComponent> texture2DComponent)
    {
        m_Textures[name] = texture2DComponent;
    }

    void MaterialComponent::AddTexture(const std::string& name, const std::string filepath)
    {
        std::shared_ptr<Texture2DComponent> texture2DComponent(new Texture2DComponent(m_EntityHandle));
        texture2DComponent->SetImage(filepath);
        m_Textures[name] = texture2DComponent;
    }

    void MaterialComponent::SetShader(std::shared_ptr<ShaderLibrary> shaderLibrary, const std::string& shaderName)
    {
        m_ShaderLibrary = shaderLibrary;
        m_ShaderName = shaderName;
    }

    const std::unordered_map<std::string, std::shared_ptr<Texture2DComponent>>& MaterialComponent::GetTextures() const
    {
        return m_Textures;
    }

    const glm::vec3& MaterialComponent::GetEmissive() const
    {
        return m_Emissive;
    }

    std::shared_ptr<Shader> MaterialComponent::GetShader() const
    {
        return m_ShaderLibrary->Get(m_ShaderName);
    }

    const std::shared_ptr<ShaderLibrary>& MaterialComponent::GetShaderLibrary() const
    {
        return m_ShaderLibrary;
    }
    const std::string& MaterialComponent::GetShaderName() const
    {
        return m_ShaderName;
    }
    std::string MaterialComponent::GetShaderNameCopy()
    {
        return m_ShaderName;
    }
}

