#include "Texture2DComponent.h"

namespace HE
{
    Texture2DComponent::Texture2DComponent():
        m_Texture(nullptr)
    {
        m_EntityHandle = nullptr;
        m_Type = ComponentType::Texture2DComponent;
    }

    Texture2DComponent::Texture2DComponent(Entity* entityHandle)
    {
        m_EntityHandle = entityHandle;
        m_Type = ComponentType::Texture2DComponent;
    }

    Texture2DComponent::Texture2DComponent(Entity* entityHandle, const std::string& filepath):
        m_Texture(Texture2D::Create(filepath))
    {
        m_EntityHandle = entityHandle;
        m_Type = ComponentType::Texture2DComponent;
    }

    Texture2DComponent::Texture2DComponent(Entity* entityHandle, std::shared_ptr<Texture2D> texture):
        m_Texture(texture)
    {
        m_EntityHandle = entityHandle;
        m_Type = ComponentType::Texture2DComponent;
    }

    void Texture2DComponent::SetImage(std::shared_ptr<Texture2D> texture)
    {
        m_Texture = texture;
    }

    const Texture2D& Texture2DComponent::GetTexture() const
    {
        return *m_Texture;
    }

    void Texture2DComponent::Bind(uint32_t id) const
    {
        m_Texture->Bind(id);
    }

}


