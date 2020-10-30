#ifndef SCENECAMERA_H
#define SCENECAMERA_H

#include "HartEng/Renderer/Cameras.h"

// This camera used as runtime camera (it is part of Camera Component)
namespace HE
{
    class SceneCamera: public Camera
    {
    public:
        enum class ProjectionType
        {
            Perspective = 0,
            Orthographic = 1
        };

    private:
        ProjectionType m_ProjectionType = ProjectionType::Orthographic;

        float m_PerspectiveFOV = 45.0f;
        float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

        float m_OrthographicSize = 10.0f;
        float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

        float m_AspectRatio = 1.0f;


        void RecalculateProjection();
    public:


        SceneCamera();
        virtual ~SceneCamera() = default;

        void SetPerspective(float fov, float nearClip, float farClip);
        void SetPerspective(float fov, float aspectRatio, float nearClip, float farClip);
        void SetOrthographic(float size, float nearClip, float farClip);
        void SetViewportSize(uint32_t width, uint32_t height);
        void SetProjectionType(ProjectionType type);

        ProjectionType GetProjectionType() const;
    };
}


#endif // SCENECAMERA_H
