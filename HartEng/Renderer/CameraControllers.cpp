#include "HartEng/Renderer/CameraControllers.h"
#include "HartEng/Core/Input.h"
#include "HartEng/Core/Keycodes.h"
#include "HartEng/Core/pch.h"
#include "HartEng/Core/Log.h"

#include <iostream>
#include <string>
namespace HE
{
/////////////////////////////////////////// Orthographic Camera /////////////////////////////////////////////

    OrthographicCameraController::OrthographicCameraController(float width, float height, bool rotation):
        m_AspectRatio(width / height),
        m_Rotation(rotation),
        m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
    {
        HE_PROFILE_FUNCTION();
    }


    void OrthographicCameraController::OnUpdate(Timestep ts)
    {
        HE_PROFILE_FUNCTION();

        glm::vec3 cameraPosition = m_Camera.GetPosition();
        float cameraRotation = m_Camera.GetRotation();
        // TODO поставить кастомные коды
        if (Input::IsKeyPressed(HE_KEY_A))
        {
            cameraPosition.x -= cos(glm::radians(cameraRotation)) * m_CameraTranslationSpeed * ts;
            cameraPosition.y -= sin(glm::radians(cameraRotation)) * m_CameraTranslationSpeed * ts;
        }
        else if (Input::IsKeyPressed(HE_KEY_D))
        {
            cameraPosition.x += cos(glm::radians(cameraRotation)) * m_CameraTranslationSpeed * ts;
            cameraPosition.y += sin(glm::radians(cameraRotation)) * m_CameraTranslationSpeed * ts;
        }

        if (Input::IsKeyPressed(HE_KEY_W))
        {
            cameraPosition.x += -sin(glm::radians(cameraRotation)) * m_CameraTranslationSpeed * ts;
            cameraPosition.y += cos(glm::radians(cameraRotation)) * m_CameraTranslationSpeed * ts;
        }
        else if (Input::IsKeyPressed(HE_KEY_S))
        {
            cameraPosition.x -= -sin(glm::radians(cameraRotation)) * m_CameraTranslationSpeed * ts;
            cameraPosition.y -= cos(glm::radians(cameraRotation)) * m_CameraTranslationSpeed * ts;
        }
        if (m_Rotation)
        {
            if (Input::IsKeyPressed(HE_KEY_Q))
                cameraRotation += m_CameraRotationSpeed * ts;
            if (Input::IsKeyPressed(HE_KEY_E))
                cameraRotation -= m_CameraRotationSpeed * ts;

            if (cameraRotation > 180.0f)
                cameraRotation -= 360.0f;
            else if (cameraRotation <= -180.0f)
                cameraRotation += 360.0f;

            m_Camera.SetRotation(cameraRotation);
        }

        m_Camera.SetPosition(cameraPosition);
        m_CameraTranslationSpeed = m_ZoomLevel;
    }

    void OrthographicCameraController::OnEvent(Event& e)
    {
        HE_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(HE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScroll));
        dispatcher.Dispatch<WindowResizeEvent>(HE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    bool OrthographicCameraController::OnMouseScroll(MouseScrolledEvent& e)
    {
        HE_PROFILE_FUNCTION();

        m_ZoomLevel -= e.GetYOffset() * 0.25f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }

    void OrthographicCameraController::OnResize(float width, float height)
    {
        m_AspectRatio = width / height;
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
    {
        HE_PROFILE_FUNCTION();

        OnResize((float)e.GetWidth(), (float)e.GetHeight());

        return false;
    }


    /////////////////////////////////////////// Perspective Camera /////////////////////////////////////////////

    PerspectiveCameraController::PerspectiveCameraController(float fov, float width, float height, float zNear, float zFar):
        m_AspectRatio(width / height),
        m_Fov(fov),
        m_LastMousePosition(width / 2.0f, height / 2.0f),
        m_Near(zNear),
        m_Far(zFar),
        m_Camera(fov, m_AspectRatio, zNear, zFar)
    {
        HE_PROFILE_FUNCTION();

        m_Front = {0.0f, 1.0f, -1.0f};
        m_Up = {0.0f, 1.0f, 0.0f};
        m_WorldUp = m_Up;
        m_Right = {1.0f, 0.0f, 0.0f};
    }

    PerspectiveCameraController::PerspectiveCameraController(float fov, float aspectRatio, float zNear, float zFar):
        m_AspectRatio(aspectRatio),
        m_Fov(fov),
        m_LastMousePosition(0.0f, 0.0f),
        m_Near(zNear),
        m_Far(zFar),
        m_Camera(fov, m_AspectRatio, zNear, zFar)
    {
        HE_PROFILE_FUNCTION();

        m_Front = {0.0f, 1.0f, -1.0f};
        m_Up = {0.0f, 1.0f, 0.0f};
        m_WorldUp = m_Up;
        m_Right = {1.0f, 0.0f, 0.0f};
    }


    void PerspectiveCameraController::OnUpdate(Timestep ts)
    {
        HE_PROFILE_FUNCTION();

        // TODO поставить кастомные коды
        {
            HE_PROFILE_SCOPE("Rotation - PerspectiveCameraController::OnUpdate");
            // Rotation
            auto [x, y] = Input::GetMousePosition();

            glm::vec2 offset(x - m_LastMousePosition.x, m_LastMousePosition.y - y);

            m_LastMousePosition.x = x;
            m_LastMousePosition.y = y;

            offset *= m_CameraSensivity;

            // TODO quaternion realization
            // euler angles realization

            m_Yaw += glm::radians(offset.x);
            m_Pitch += glm::radians(offset.y);
            m_Front = glm::normalize(glm::vec3(glm::cos(m_Yaw) * glm::cos(m_Pitch), glm::sin(m_Pitch), glm::sin(m_Yaw) * glm::cos(m_Pitch)));
            m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
            m_Up = glm::normalize(glm::cross(m_Right, m_Front));

            glm::quat cameraRotation = m_Camera.GetRotation();
            cameraRotation = glm::quat(glm::vec3(m_Pitch, m_Yaw, 0.0f));

            //m_Camera.SetRotation(cameraRotation);

        }

        {
            glm::vec3 cameraPosition = m_Camera.GetPosition();
            HE_PROFILE_SCOPE("Position - PerspectiveCameraController::OnUpdate");
            // Position
            float velocity = m_CameraTranslationSpeed * ts;
            if (Input::IsKeyPressed(HE_KEY_W)) // forawrd
            {
                cameraPosition += m_Front * velocity;
            }
            if (Input::IsKeyPressed(HE_KEY_S)) // backward
            {
                cameraPosition -= m_Front * velocity;
            }
            if (Input::IsKeyPressed(HE_KEY_D)) // right
            {
                cameraPosition += m_Right * velocity;
            }
            if (Input::IsKeyPressed(HE_KEY_A)) // left
            {
                cameraPosition -= m_Right * velocity;
            }
            if (Input::IsKeyPressed(HE_KEY_Q)) // up
            {
                cameraPosition += m_Up * velocity;
            }
            if (Input::IsKeyPressed(HE_KEY_E)) // down
            {
                cameraPosition -= m_Up * velocity;
            }
            m_Camera.SetPosition(cameraPosition);
            m_Camera.RecalculateView(m_Front, m_Up); // dirty hack
        }



    }

    void PerspectiveCameraController::OnEvent(Event& e)
    {
        HE_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(HE_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScroll));
        dispatcher.Dispatch<WindowResizeEvent>(HE_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
    }

    bool PerspectiveCameraController::OnMouseScroll(MouseScrolledEvent& e)
    {
        HE_PROFILE_FUNCTION();

        // TODO кастомные настройки fov-a
        if (m_Fov >= 1.0f && m_Fov <= 45.0f)
            m_Fov += glm::radians(e.GetYOffset());
        if (m_Fov <= 1.0f)
            m_Fov = 1.0f;
        if (m_Fov >= 45.0f)
            m_Fov = 45.0f;
        m_Camera.SetProjection(m_Fov, m_AspectRatio, m_Near, m_Far);

        return false;
    }

    void PerspectiveCameraController::OnResize(float width, float height)
    {
        m_AspectRatio = width / height;
        m_Camera.SetProjection(m_Fov, m_AspectRatio, m_Near, m_Far);
    }

    bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e)
    {
        HE_PROFILE_FUNCTION();

        OnResize((float)e.GetWidth(), (float)e.GetHeight());

        return false;
    }
}
