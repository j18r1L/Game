#include "HartEng/Renderer/CameraControllers.h"
#include "HartEng/Input.h"
#include "HartEng/Keycodes.h"
#include "HartEng/pch.h"

#include <iostream>
#include <string>
namespace HE
{


    void CameraController::SetPosition(const glm::vec3& position)
    {
        m_CameraPosition = position;
        m_Camera->RecalculateViewMatrix(m_CameraPosition, m_CameraRotation);
    }
    void CameraController::SetRotation(float angle, const glm::vec3& rotation)
    {
        m_CameraRotation = glm::angleAxis(glm::radians(angle), rotation);
        m_Camera->RecalculateViewMatrix(m_CameraPosition, m_CameraRotation);
    }


/////////////////////////////////////////// Orthographic Camera /////////////////////////////////////////////

    OrthographicCameraController::OrthographicCameraController(float width, float heigth, float zNear, float zFar):
        m_ZoomLevel(1.0f)
    {
        m_AspectRatio = (width / heigth);
        m_Znear = zNear;
        m_Zfar = zFar;
        m_Camera = std::shared_ptr<OrthographicCamera>(new OrthographicCamera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, m_Znear, m_Zfar));
    }

    OrthographicCameraController::OrthographicCameraController(float aspectRatio, float zNear, float zFar):
        m_ZoomLevel(1.0f)
    {
        m_AspectRatio = aspectRatio;
        m_Znear = zNear;
        m_Zfar = zFar;
        m_Camera = std::shared_ptr<OrthographicCamera>(new OrthographicCamera(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, m_Znear, m_Zfar));
    }

    void OrthographicCameraController::OnUpdate(Timestep ts)
    {
        // TODO поставить кастомные коды
        float deltaTime = ts;
        if (Input::IsKeyPressed(HE_KEY_W))
        {
            m_CameraPosition.z += m_CameraSpeed * deltaTime;
        }
        if (Input::IsKeyPressed(HE_KEY_S))
        {
            m_CameraPosition.z -= m_CameraSpeed * deltaTime;
        }
        if (Input::IsKeyPressed(HE_KEY_A))
        {
            m_CameraPosition.x -= m_CameraSpeed * deltaTime;
        }
        if (Input::IsKeyPressed(HE_KEY_D))
        {
            m_CameraPosition.x += m_CameraSpeed * deltaTime;
        }
        if (Input::IsKeyPressed(HE_KEY_Q))
        {
            m_CameraPosition.y += m_CameraSpeed * deltaTime;
        }
        if (Input::IsKeyPressed(HE_KEY_E))
        {
            m_CameraPosition.y -= m_CameraSpeed * deltaTime;
        }
        m_Camera->RecalculateViewMatrix(m_CameraPosition, m_CameraRotation);
    }

    void OrthographicCameraController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(HE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScroll));
        dispatcher.Dispatch<WindowResizeEvent>(HE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    bool OrthographicCameraController::OnMouseScroll(MouseScrolledEvent& e)
    {
        m_ZoomLevel -= e.GetYOffset();

        std::dynamic_pointer_cast<OrthographicCamera>(m_Camera)->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
    {
        m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
        std::dynamic_pointer_cast<OrthographicCamera>(m_Camera)->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

        return false;
    }


    /////////////////////////////////////////// Perspective Camera /////////////////////////////////////////////

    PerspectiveCameraController::PerspectiveCameraController(float fov, float width, float heigth, float zNear, float zFar):
        m_Fov(fov)
    {
        m_AspectRatio = (width / heigth);
        m_Znear = zNear;
        m_Zfar = zFar;
        m_Camera = std::shared_ptr<PerspectiveCamera>(new PerspectiveCamera(m_Fov, m_AspectRatio, m_Znear, m_Zfar));
    }

    PerspectiveCameraController::PerspectiveCameraController(float fov, float aspectRatio, float zNear, float zFar):
        m_Fov(fov)
    {
        m_AspectRatio = aspectRatio;
        m_Znear = zNear;
        m_Zfar = zFar;
        m_Camera = std::shared_ptr<PerspectiveCamera>(new PerspectiveCamera(m_Fov, m_AspectRatio, m_Znear, m_Zfar));
    }

    void PerspectiveCameraController::OnUpdate(Timestep ts)
    {
        // TODO поставить кастомные коды

        // Rotation
        auto [x, y] = Input::GetMousePosition();


        // Position
        float velocity = m_CameraSpeed * ts;
        if (Input::IsKeyPressed(HE_KEY_W))
        {
            m_CameraPosition.z += velocity;
        }
        if (Input::IsKeyPressed(HE_KEY_S))
        {
            m_CameraPosition.z -= velocity;
        }
        if (Input::IsKeyPressed(HE_KEY_D))
        {
            m_CameraPosition.x += velocity;
        }
        if (Input::IsKeyPressed(HE_KEY_A))
        {
            m_CameraPosition.x -= velocity;
        }
        if (Input::IsKeyPressed(HE_KEY_Q))
        {
            m_CameraPosition.y += velocity;
        }
        if (Input::IsKeyPressed(HE_KEY_E))
        {
            m_CameraPosition.y -= velocity;
        }


        m_Camera->RecalculateViewMatrix(m_CameraPosition, m_CameraRotation);
    }

    void PerspectiveCameraController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(HE_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScroll));
        dispatcher.Dispatch<WindowResizeEvent>(HE_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
    }

    bool PerspectiveCameraController::OnMouseScroll(MouseScrolledEvent& e)
    {
        if (m_Fov >= 1.0f && m_Fov <= 45.0f)
            m_Fov -= e.GetYOffset();
        if (m_Fov <= 1.0f)
            m_Fov = 1.0f;
        if (m_Fov >= 45.0f)
            m_Fov = 45.0f;
        std::dynamic_pointer_cast<PerspectiveCamera>(m_Camera)->SetProjection(m_Fov, m_AspectRatio, m_Znear, m_Zfar);
        return false;
    }

    bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e)
    {
        //glm::eulerAngles(m_CameraRotation);
        m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
        std::dynamic_pointer_cast<PerspectiveCamera>(m_Camera)->SetProjection(m_Fov, m_AspectRatio, m_Znear, m_Zfar);

        return false;
    }

}
