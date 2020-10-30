#include "HartEng/Renderer/Cameras.h"

#include "HartEng/Core/pch.h"
namespace HE
{
    Camera::Camera(const glm::mat4& projection):
        m_Projection(projection)
    {

    }
    const glm::mat4& Camera::GetProjection() const
    {
        return m_Projection;
    }

    /////////////////////////////////////////// Orthographic Camera /////////////////////////////////////////////

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar):
        m_Projection(glm::ortho(left, right, bottom, top, zNear, zFar)),
        m_View(1.0f)
    {
        HE_PROFILE_FUNCTION();

        m_ProjectionView = m_Projection * m_View;
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        HE_PROFILE_FUNCTION();

        m_Projection = glm::ortho(left, right, bottom, top, zNear, zFar);
        m_ProjectionView = m_Projection * m_View;
    }

    void OrthographicCamera::SetPosition(const glm::vec3& position)
    {
        m_Position = position;
        RecalculateView();
    }

    void OrthographicCamera::SetRotation(float rotation)
    {
        m_Rotation = rotation;
        RecalculateView();
    }

    void OrthographicCamera::RecalculateView()
    {
        HE_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

        m_View = glm::inverse(transform);
        m_ProjectionView = m_Projection * m_View;
    }







    /////////////////////////////////////////// Perspective Camera /////////////////////////////////////////////


    PerspectiveCamera::PerspectiveCamera(float fov, float width, float height, float zNear, float zFar):
        PerspectiveCamera(fov, width / height, zNear, zFar)
    {

    }

    PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float zNear, float zFar):
        m_Projection(glm::perspective(fov, aspectRatio, zNear, zFar)),
        m_View(1.0f)
    {
        HE_PROFILE_FUNCTION();

        m_ProjectionView = m_Projection * m_View;
    }

    void PerspectiveCamera::SetProjection(float fov, float aspectRatio, float zNear, float zFar)
    {
        HE_PROFILE_FUNCTION();

        m_Projection = glm::perspective(fov, aspectRatio, zNear, zFar);
        m_ProjectionView = m_Projection * m_View;
    }

    void PerspectiveCamera::SetPosition(const glm::vec3& position)
    {
        m_Position = position;
        RecalculateView();
    }

    void PerspectiveCamera::SetRotation(const glm::vec3& angles)
    {
        HE_PROFILE_FUNCTION();

        m_Rotation = glm::quat(angles); // TODO make proper quaternion realization
        //m_Rotation = rotation; //euler realization
        RecalculateView();

    }

    void PerspectiveCamera::SetRotation(const glm::quat& rotation)
    {
        HE_PROFILE_FUNCTION();

        m_Rotation = rotation; // TODO make proper quaternion realization
        //m_Rotation = rotation; //euler realization
        RecalculateView();

    }

    void PerspectiveCamera::RecalculateView(glm::vec3 front, glm::vec3 up)
    {
        HE_PROFILE_FUNCTION();

        m_View = glm::lookAt(m_Position, m_Position + front, up);
        m_ProjectionView = m_Projection * m_View;
    }

    void PerspectiveCamera::RecalculateView()
    {
        HE_PROFILE_FUNCTION();
        //glm::look
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::mat4_cast(m_Rotation);
        m_View = glm::inverse(transform);
        m_ProjectionView = m_Projection * m_View;
    }
}
