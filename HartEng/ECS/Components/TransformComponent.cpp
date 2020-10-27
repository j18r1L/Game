#include "TransformComponent.h"

namespace HE
{
    TransformComponent::TransformComponent(Entity* entityHandle):
        m_Transform(1.0f)
    {
        m_EntityHandle = entityHandle;
        m_Type = ComponentType::TransformComponent;
    }
    TransformComponent::TransformComponent(Entity* entityHandle, const glm::mat4& transform):
        m_Transform(transform)
    {
        m_EntityHandle = entityHandle;
        m_Type = ComponentType::TransformComponent;
    }

    void TransformComponent::Recalculate()
    {
        m_Transform = glm::translate(glm::mat4(1.0f), m_Translation) *
                glm::mat4_cast(m_Rotation) *
                glm::scale(glm::mat4(1.0f), m_Scale);
    }

    void TransformComponent::SetTranslation(const glm::vec3& translate)
    {
        m_Translation = translate;
        Recalculate();
    }

    void TransformComponent::SetRotation(const glm::vec3& angles)
    {
        m_Rotation = glm::quat(glm::radians(angles));
        Recalculate();
    }

    void TransformComponent::SetRotation(const glm::quat& rotation)
    {
        m_Rotation = rotation;
        Recalculate();
    }
    void TransformComponent::SetScale(const glm::vec3& scale)
    {
        m_Scale = scale;
        Recalculate();
    }
    void TransformComponent::SetTransform(const glm::mat4& transform)
    {
        m_Transform = transform;
    }
    void TransformComponent::SetTRC(const glm::vec3& translate, const glm::vec3& angles, const glm::vec3& scale)
    {
        m_Translation = translate;
        m_Rotation = glm::quat(glm::radians(angles));
        m_Scale = scale;
        Recalculate();
    }
    const glm::vec3& TransformComponent::GetTranslation() const
    {
        return m_Translation;
    }
    const glm::quat& TransformComponent::GetRotation() const
    {
        return m_Rotation;
    }
    const glm::vec3& TransformComponent::GetScale() const
    {
        return m_Scale;
    }
    const glm::mat4& TransformComponent::GetTransform() const
    {
        return m_Transform;
    }
}
