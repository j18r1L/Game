#include "OrthographicCamera.h"

#include "HartEng/pch.h"
namespace HE
{

    void OrthographicCamera::SetPosition(const glm::vec3& position)
    {
        m_Position = position;
        RecalculateViewMatrix();
    }
    void OrthographicCamera::SetRotation(float angle, const glm::vec3& rotation)
    {
        // angle в градусах
        m_Rotation = glm::angleAxis(angle, rotation);
        RecalculateViewMatrix();
    }

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top):
        m_ProjectionMatrix(glm::ortho(left, right, bottom, top)),
        m_ViewMatrix(1.0f)
    {
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float near, float far):
        m_ProjectionMatrix(glm::ortho(left, right, bottom, top, near, far)),
        m_ViewMatrix(1.0f)
    {
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }



    void OrthographicCamera::RecalculateViewMatrix()
    {
        m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::mat4_cast(m_Rotation);
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    const glm::vec3& OrthographicCamera::GetPosition() const
    {
        return m_Position;
    }

    const glm::quat& OrthographicCamera::GetRotation() const
    {
        return m_Rotation;
    }

    const glm::mat4& OrthographicCamera::GetProjectionMatrix() const
    {
        return m_ProjectionMatrix;
    }
    const glm::mat4& OrthographicCamera::GetViewMatrix() const
    {
        return m_ViewMatrix;
    }
    const glm::mat4& OrthographicCamera::GetProjectionViewMatrix() const
    {
        return m_ProjectionViewMatrix;
    }

}
