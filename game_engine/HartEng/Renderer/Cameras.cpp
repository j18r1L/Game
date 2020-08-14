#include "HartEng/Renderer/Cameras.h"

#include "HartEng/pch.h"
namespace HE
{

    void Camera::SetPosition(const glm::vec3& position)
    {
        m_Position = position;
        RecalculateViewMatrix();
    }
    void Camera::SetRotation(float angle, const glm::vec3& rotation)
    {
        m_Rotation = glm::angleAxis(glm::radians(angle), rotation);
        RecalculateViewMatrix();
    }

    const glm::vec3& Camera::GetPosition() const
    {
        return m_Position;
    }

    const glm::quat& Camera::GetRotation() const
    {
        return m_Rotation;
    }

    const glm::mat4& Camera::GetProjectionMatrix() const
    {
        return m_ProjectionMatrix;
    }
    const glm::mat4& Camera::GetViewMatrix() const
    {
        return m_ViewMatrix;
    }
    const glm::mat4& Camera::GetProjectionViewMatrix() const
    {
        return m_ProjectionViewMatrix;
    }

    void Camera::RecalculateViewMatrix()
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::mat4_cast(m_Rotation);
        m_ViewMatrix = glm::inverse(transform);
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    /////////////////////////////////////////// Orthographic Camera /////////////////////////////////////////////

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float near, float far)
    {
        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, near, far);
        m_ViewMatrix = glm::mat4(1.0f);
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }





    /////////////////////////////////////////// Perspective Camera /////////////////////////////////////////////


    PerspectiveCamera::PerspectiveCamera(float fov, float width, float heigth, float near, float far)

    {
        m_ProjectionMatrix = glm::perspective(fov, width / heigth, near, far);
        m_ViewMatrix = glm::mat4(1.0f);
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    PerspectiveCamera::PerspectiveCamera(float fov, float aspect_ratio, float near, float far)
    {
        m_ProjectionMatrix = glm::perspective(fov,aspect_ratio, near, far);
        m_ViewMatrix = glm::mat4(1.0f);
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }


}
