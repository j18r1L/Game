#pragma once

#include "HartEng/Renderer/Cameras.h"
#include "HartEng/Core/Timestep.h"

#include "HartEng/Events/Event.h"
#include "HartEng/Events/ApplicationEvent.h"
#include "HartEng/Events/MousesEvent.h"

namespace HE
{

    /////////////////////////////////////////// Orthographic Camera /////////////////////////////////////////////


    class OrthographicCameraController
    {
    private:
        float m_AspectRatio = 0.0f;
        float m_ZoomLevel = 1.0f;

        bool m_Rotation = false;

        float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;

        OrthographicCamera m_Camera;

        bool OnMouseScroll(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);

    public:
        OrthographicCameraController(float width, float height, bool rotation);
        OrthographicCameraController(float aspectRatio, bool rotation);

        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);
        void OnResize(float width, float height);

        OrthographicCamera& GetCamera() { return m_Camera; }
        const OrthographicCamera& GetCamera() const { return m_Camera; }

        float GetZoomLevel() const { return m_ZoomLevel; }
        void SetZoomLevel(float level) { m_ZoomLevel = level; }
    };

    /////////////////////////////////////////// Perspective Camera /////////////////////////////////////////////


    class PerspectiveCameraController
    {
    private:
        float m_AspectRatio = 0.0f;
        float m_Fov = 45.0f;
        float m_CameraSensivity = 0.15f;
        float m_CameraTranslationSpeed = 5.0f;

        glm::vec2 m_LastMousePosition;

        glm::vec3 m_Front = {0.0f, 1.0f, -1.0f};
        glm::vec3 m_Up = {0.0f, 1.0f, 0.0f};
        glm::vec3 m_WorldUp = {0.0f, 1.0f, 0.0f};
        glm::vec3 m_Right = {1.0f, 0.0f, 0.0f};


        float m_Yaw = 0.0f;
        float m_Pitch = 0.0f;
        float m_Near = 0.1f;
        float m_Far = 1000.0f;

        float m_Width = 0.0f;
        float m_Height = 0.0f;

        PerspectiveCamera m_Camera;

        bool OnMouseScroll(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);

    public:
        PerspectiveCameraController(float fov, float width, float height, float zNear, float zFar);

        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);
        void OnResize(float width, float height);

        PerspectiveCamera& GetCamera() { return m_Camera; }
        const PerspectiveCamera& GetCamera() const { return m_Camera; }
        float GetWidth() const { return m_Width; }
        float GetHeight() const { return m_Height; }

    };
}
