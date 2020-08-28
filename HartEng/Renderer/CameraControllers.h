#ifndef CAMERACONTROLLERS_H
#define CAMERACONTROLLERS_H

#include "HartEng/Renderer/Cameras.h"
#include "HartEng/Core/Timestep.h"

#include "HartEng/Events/Event.h"
#include "HartEng/Events/ApplicationEvent.h"
#include "HartEng/Events/MousesEvent.h"

namespace HE
{
    class CameraController
    {
    protected:
        glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
        glm::quat m_CameraRotation;
        float m_CameraSpeed = 1.0f;
        float m_Znear = 0.0f, m_Zfar = 10.0f;

        float m_AspectRatio;

        std::shared_ptr<Camera> m_Camera;

    public:
        virtual void OnUpdate(Timestep ts) = 0;
        virtual void OnEvent(Event& e) = 0;

        void SetPosition(const glm::vec3& position);
        void SetRotation(float angle, const glm::vec3& rotation);


        virtual std::shared_ptr<Camera> GetCamera()
        {
            return m_Camera;
        }

        virtual const std::shared_ptr<Camera> GetCamera() const
        {
            return m_Camera;
        }
        const glm::vec3& GetPosition() const
        {
            return m_CameraPosition;
        }
        const glm::quat& GetRotation() const
        {
            return m_CameraRotation;
        }

    };

    /////////////////////////////////////////// Orthographic Camera /////////////////////////////////////////////


    class OrthographicCameraController: public CameraController
    {
    private:
        float m_ZoomLevel;

        bool OnMouseScroll(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);

    public:
        OrthographicCameraController(float width, float heigth, float zNear = -1.0f, float zFar = 1.0f);
        OrthographicCameraController(float aspectRatio, float zNear = -1.0f, float zFar = 1.0f);

        virtual void OnUpdate(Timestep ts) override;
        virtual void OnEvent(Event& e) override;
    };

    /////////////////////////////////////////// Perspective Camera /////////////////////////////////////////////


    class PerspectiveCameraController: public CameraController
    {
    private:
        float m_Fov;
        float m_CameraSensivity;

        glm::vec2 m_LastMousePosition;

        glm::vec3 m_Front;
        glm::vec3 m_Up;
        glm::vec3 m_WorldUp;
        glm::vec3 m_Right;


        float m_Yaw = 0.0f;
        float m_Pitch = 0.0f;

        bool OnMouseScroll(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);

    public:
        PerspectiveCameraController(float fov, float aspect_ratio, float zNear, float zFar);
        PerspectiveCameraController(float fov, float width, float heigth, float zNear, float zFar);

        virtual void OnUpdate(Timestep ts) override;
        virtual void OnEvent(Event& e) override;

    };



}


#endif // CAMERACONTROLLERS_H
