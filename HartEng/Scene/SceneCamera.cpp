#include "HartEng/Scene/SceneCamera.h"
#include "HartEng/Core/Log.h"

namespace HE
{
    SceneCamera::SceneCamera()
    {
        RecalculateProjection();
    }

    void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
    {
        HE_PROFILE_FUNCTION();

        m_ProjectionType = ProjectionType::Perspective;
        m_PerspectiveFOV = fov;
        m_PerspectiveNear = nearClip;
        m_PerspectiveFar = farClip;
        RecalculateProjection();
    }

    void SceneCamera::SetPerspective(float fov, float aspectRatio, float nearClip, float farClip)
    {
        HE_PROFILE_FUNCTION();

        m_ProjectionType = ProjectionType::Perspective;
        m_PerspectiveFOV = fov;
        m_PerspectiveNear = nearClip;
        m_PerspectiveFar = farClip;
        m_AspectRatio = aspectRatio;
        RecalculateProjection();
    }

    void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
    {
        HE_PROFILE_FUNCTION();

        m_ProjectionType = ProjectionType::Orthographic;
        m_OrthographicSize = size;
        m_OrthographicNear = nearClip;
        m_OrthographicFar = farClip;
        RecalculateProjection();
    }

    void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
    {
        HE_PROFILE_FUNCTION();

        m_AspectRatio = (float)width / (float)height;
        RecalculateProjection();
    }

    void SceneCamera::SetProjectionType(ProjectionType type)
    {
        HE_PROFILE_FUNCTION();

        m_ProjectionType = type;
        RecalculateProjection();
    }

    void SceneCamera::RecalculateProjection()
    {
        HE_PROFILE_FUNCTION();

        if (m_ProjectionType == ProjectionType::Perspective)
        {
            m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
        }
        else
        {
            float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
            float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
            float orthoBottom = -m_OrthographicSize * 0.5f;
            float orthoTop = m_OrthographicSize * 0.5f;

            m_Projection = glm::ortho(orthoLeft, orthoRight,
                    orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
        }

    }
}

