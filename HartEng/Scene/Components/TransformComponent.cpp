#include "TransformComponent.h"
#include <glm/gtx/euler_angles.hpp>

namespace HE
{
    TransformComponent::TransformComponent(Entity* entityHandle):
        m_Transform(1.0f)
    {
        m_EntityHandle = entityHandle;
        m_Type = ComponentType::TransformComponent;
    }

    void TransformComponent::Recalculate()
    {
        m_Transform = glm::translate(glm::mat4(1.0f), m_Position) *
                glm::orientate4(m_Rotation) *
                glm::scale(glm::mat4(1.0f), m_Scale);
    }

    void TransformComponent::SetPosition(const glm::vec3& position)
    {
        m_Position = position;
        Recalculate();
    }

    void TransformComponent::SetRotation(const glm::vec3& angles)
    {
        m_Rotation = glm::radians(angles);
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
    void TransformComponent::SetPRS(const glm::vec3& position, const glm::vec3& angles, const glm::vec3& scale)
    {
        m_Position = position;
        m_Rotation = glm::radians(angles);
        m_Scale = scale;
        Recalculate();
    }
    const glm::vec3& TransformComponent::GetPosition() const
    {
        return m_Position;
    }
    const glm::vec3& TransformComponent::GetRotation() const
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
