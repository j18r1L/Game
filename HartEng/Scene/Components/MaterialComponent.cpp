#include "MaterialComponent.h"

namespace HE
{

    MaterialComponent::MaterialComponent():
        m_Textures(),
        m_Emissive(0.0f)
    {
        m_EntityHandle = nullptr;
        m_Type = ComponentType::MaterialComponent;
    }

    MaterialComponent::MaterialComponent(Entity* entityHandle)
    {
        m_EntityHandle = entityHandle;
        m_Type = ComponentType::MaterialComponent;
    }

    MaterialComponent::MaterialComponent(Entity* entityHandle, const glm::vec3& emissive):
        m_Emissive(emissive)
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
        m_Textures[name] = std::make_shared<Texture2DComponent>(m_EntityHandle, filepath);
    }

    const std::unordered_map<std::string, std::shared_ptr<Texture2DComponent>>& MaterialComponent::GetTextures() const
    {
        return m_Textures;
    }

    const glm::vec3& MaterialComponent::GetEmissive() const
    {
        return m_Emissive;
    }
}

