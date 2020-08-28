#include "HartEng/Renderer/Cameras.h"

#include "HartEng/Core/pch.h"
namespace HE
{
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

    void Camera::RecalculateViewMatrix(glm::vec3 position, glm::quat rotation)
    {
        HE_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation);
        m_ViewMatrix = glm::inverse(transform);
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void Camera::RecalculateViewMatrix(glm::vec3 position, glm::vec3 front, glm::vec3 up)
    {
        HE_PROFILE_FUNCTION();

        m_ViewMatrix = glm::lookAt(position, position + front, up);
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    /////////////////////////////////////////// Orthographic Camera /////////////////////////////////////////////

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        HE_PROFILE_FUNCTION();

        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
        m_ViewMatrix = glm::mat4(1.0f);
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        HE_PROFILE_FUNCTION();

        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }





    /////////////////////////////////////////// Perspective Camera /////////////////////////////////////////////


    PerspectiveCamera::PerspectiveCamera(float fov, float width, float heigth, float zNear, float zFar)
    {
        HE_PROFILE_FUNCTION();

        m_ProjectionMatrix = glm::perspective(fov, width / heigth, zNear, zFar);
        m_ViewMatrix = glm::mat4(1.0f);
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float zNear, float zFar)
    {
        HE_PROFILE_FUNCTION();

        m_ProjectionMatrix = glm::perspective(fov, aspectRatio, zNear, zFar);
        m_ViewMatrix = glm::mat4(1.0f);
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void PerspectiveCamera::SetProjection(float fov, float aspectRatio, float zNear, float zFar)
    {
        HE_PROFILE_FUNCTION();

        m_ProjectionMatrix = glm::perspective(fov, aspectRatio, zNear, zFar);
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }


}
