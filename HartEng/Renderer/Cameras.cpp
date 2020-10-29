#include "HartEng/Renderer/Cameras.h"

#include "HartEng/Core/pch.h"
namespace HE
{
    Camera::Camera(const glm::mat4& projection):
        m_Projection(projection),
        m_View(1.0f),
        m_ProjectionView(projection * m_View)
    {

    }
    const glm::mat4& Camera::GetProjection() const
    {
        return m_Projection;
    }
    const glm::mat4& Camera::GetView() const
    {
        return m_View;
    }
    const glm::mat4& Camera::GetProjectionView() const
    {
        return m_ProjectionView;
    }

    void Camera::RecalculateView(glm::vec3 position, glm::quat rotation)
    {
        HE_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation);
        m_View = glm::inverse(transform);
        m_ProjectionView = m_Projection * m_View;
    }

    void Camera::RecalculateView(glm::vec3 position, glm::vec3 front, glm::vec3 up)
    {
        HE_PROFILE_FUNCTION();

        m_View = glm::lookAt(position, position + front, up);
        m_ProjectionView = m_Projection * m_View;
    }

    /////////////////////////////////////////// Orthographic Camera /////////////////////////////////////////////

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float nearClip, float farClip):
        m_Left(left),
        m_Right(right),
        m_Bottom(bottom),
        m_Top(top),
        m_Near(nearClip),
        m_Far(farClip)
    {
        HE_PROFILE_FUNCTION();

        m_Projection = glm::ortho(left, right, bottom, top, nearClip, farClip);
        m_View = glm::mat4(1.0f);
        m_ProjectionView = m_Projection * m_View;
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top, float nearClip, float farClip)
    {
        HE_PROFILE_FUNCTION();

        m_Projection = glm::ortho(left, right, bottom, top, nearClip, farClip);
        m_ProjectionView = m_Projection * m_View;
    }

    void OrthographicCamera::SetViewportSize(uint32_t width, uint32_t height)
    {
        HE_PROFILE_FUNCTION();

        m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);
        float orthoLeft = -m_AspectRatio * glm::abs(m_Left);
        float orthoRight = -m_AspectRatio * glm::abs(m_Right);

        m_Projection = glm::ortho(orthoLeft, orthoRight, m_Bottom, m_Top, m_Near, m_Far);
        m_ProjectionView = m_Projection * m_View;
    }





    /////////////////////////////////////////// Perspective Camera /////////////////////////////////////////////


    PerspectiveCamera::PerspectiveCamera(float fov, float width, float heigth, float zNear, float zFar):
        m_Fov(fov),
        m_Near(zNear),
        m_Far(zFar)
    {
        HE_PROFILE_FUNCTION();

        m_Projection = glm::perspective(fov, width / heigth, zNear, zFar);
        m_View = glm::mat4(1.0f);
        m_ProjectionView = m_Projection * m_View;
    }

    PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float zNear, float zFar):
        m_Fov(fov),
        m_Near(zNear),
        m_Far(zFar)
    {
        HE_PROFILE_FUNCTION();

        m_Projection = glm::perspective(fov, aspectRatio, zNear, zFar);
        m_View = glm::mat4(1.0f);
        m_ProjectionView = m_Projection * m_View;
    }

    void PerspectiveCamera::SetProjection(float fov, float aspectRatio, float zNear, float zFar)
    {
        HE_PROFILE_FUNCTION();

        m_Projection = glm::perspective(fov, aspectRatio, zNear, zFar);
        m_ProjectionView = m_Projection * m_View;
    }

    void PerspectiveCamera::SetViewportSize(uint32_t width, uint32_t height)
    {
        HE_PROFILE_FUNCTION();

        m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);
        m_Projection = glm::perspective(m_Fov, m_AspectRatio, m_Near, m_Far);
        m_ProjectionView = m_Projection * m_View;
    }


}
