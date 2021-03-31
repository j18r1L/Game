#pragma once

#include "HartEng/Renderer/Cameras.h"

// This camera used as runtime camera (it is part of Camera Component)
namespace HE
{
    class SceneCamera: public Camera
    {
    public:
        enum ProjectionType
        {
            Perspective = 0,
            Orthographic = 1
        };

    private:
        ProjectionType m_ProjectionType = ProjectionType::Perspective;

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

        void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection();}
        void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection();}
        void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjection();}
        void SetPerspectiveFov(float fov) { m_PerspectiveFOV = fov; RecalculateProjection();}
        void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection();}
        void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection();}


        float GetOrthographicSize() const { return m_OrthographicSize; }
        float GetOrthographicNearClip() const { return m_OrthographicNear; }
        float GetOrthographicFarClip() const { return m_OrthographicFar; }
        float GetPerspectiveFov() const { return m_PerspectiveFOV; }
        float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
        float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
        float GetAspectRatio() const { return m_AspectRatio; }


        ProjectionType GetProjectionType() const
        {
            return m_ProjectionType;
        }
    };
}

