#include "TransformComponent.h"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>
namespace HE
{
    TransformComponent::TransformComponent(Entity* entityHandle):
        m_Transform(1.0f)
    {
        m_EntityHandle = entityHandle;
    }

    void TransformComponent::Copy(Component* other_base)
    {
        TransformComponent* other = dynamic_cast<TransformComponent*>(other_base);
        m_Transform = other->m_Transform;
        m_Position = other->m_Position;
        m_Rotation = other->m_Rotation;
        m_Scale = other->m_Scale;
    }

    void TransformComponent::Recalculate()
    {
        
        m_Transform = glm::translate(glm::mat4(1.0f), m_Position) *
                glm::orientate4(m_Rotation) *
                glm::scale(glm::mat4(1.0f), m_Scale);
        
        /*
        m_Transform = glm::translate(glm::mat4(1.0f), m_Position) *
            glm::mat4_cast(m_Rotation) *
            glm::scale(glm::mat4(1.0f), m_Scale);
        */
    }

    void TransformComponent::SetPosition(const glm::vec3& position)
    {
        m_Position = position;
        Recalculate();
    }
    
    void TransformComponent::SetRotation(const glm::vec3& angles)
    {
        //m_Rotation = glm::quat(glm::radians(angles));
        //glm::degrees(glm::eulerAngles(glm::normalize(quaternion)))
        m_Rotation = glm::radians(angles);
        Recalculate();
    }
    /*
    void TransformComponent::SetRotation(const glm::quat& quaternion)
    {
        m_Rotation = quaternion;
        Recalculate();
    }
    */

    void TransformComponent::SetScale(const glm::vec3& scale)
    {
        m_Scale = scale;
        Recalculate();
    }
    void TransformComponent::SetTransform(const glm::mat4& transform)
    {
        m_Transform = transform;
        /*
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::quat rotation;
        glm::decompose(m_Transform, m_Scale, rotation, m_Position, skew, perspective);
        m_Rotation = glm::degrees(glm::eulerAngles(rotation));
        */
    }
    void TransformComponent::SetPRS(const glm::vec3& position, const glm::vec3& angles, const glm::vec3& scale)
    {
        m_Position = position;
        //m_Rotation = glm::quat(glm::radians(angles));
        m_Rotation = glm::radians(angles);
        m_Scale = scale;
        Recalculate();
    }
    const glm::vec3& TransformComponent::GetPosition() const
    {
        return m_Position;
    }
    /*
    const glm::quat& TransformComponent::GetRotationQ() const
    {
        return m_Rotation;
    }
    */
    const glm::vec3& TransformComponent::GetRotation() const
    {
        //return glm::eulerAngles(m_Rotation);
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
