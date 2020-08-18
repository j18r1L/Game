#include "HartEng/Renderer/CameraControllers.h"
#include "HartEng/Input.h"
#include "HartEng/Keycodes.h"
#include "HartEng/pch.h"
#include "HartEng/Log.h"

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
        m_Fov(fov),
        m_CameraSensivity(0.05f),
        m_LastMousePosition(width / 2, heigth / 2),
        m_Up(0.0f, 1.0f, 0.0f),
        m_Front(glm::vec3(0.0f, 1.0f, -1.0f)),
        m_Right(glm::vec3(1.0f, 0.0f, 0.0f)),
        m_WorldUp(m_Up)
    {
        m_CameraRotation = glm::quat();
        //glm::vec3 angles = glm::eulerAngles(m_CameraRotation) * 3.14159f / 180.f; //XYZ as pitch, yaw, and roll
        //m_Front = glm::normalize(glm::vec3(glm::cos(angles.y) * glm::cos(angles.x), glm::sin(angles.x), glm::sin(angles.y) * glm::cos(angles.x)));
        //m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
        //m_Up = glm::normalize(glm::cross(m_Right, m_Front));

        m_AspectRatio = (width / heigth);
        m_Znear = zNear;
        m_Zfar = zFar;
        m_Camera = std::shared_ptr<PerspectiveCamera>(new PerspectiveCamera(m_Fov, m_AspectRatio, m_Znear, m_Zfar));
    }

    PerspectiveCameraController::PerspectiveCameraController(float fov, float aspectRatio, float zNear, float zFar):
        m_Fov(fov),
        m_CameraSensivity(0.05f),
        m_LastMousePosition(0.0f, 0.0f), // TODO возможно надо изменить, так как мышка сперва оказывается не в 0.0, 0.0
        m_Up(0.0f, 1.0f, 0.0f),
        m_Front(glm::vec3(0.0f, 1.0f, -1.0f)),
        m_Right(glm::vec3(1.0f, 0.0f, 0.0f)),
        m_WorldUp(m_Up)
    {
        m_CameraRotation = glm::quat();
        //glm::vec3 angles = glm::eulerAngles(m_CameraRotation) * 3.14159f / 180.f; //XYZ as pitch, yaw, and roll
        //m_Front = glm::normalize(glm::vec3(glm::cos(angles.y) * glm::cos(angles.x), glm::sin(angles.x), glm::sin(angles.y) * glm::cos(angles.x)));
        //m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
        //m_Up = glm::normalize(glm::cross(m_Right, m_Front));


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

        glm::vec2 offset(x - m_LastMousePosition.x, m_LastMousePosition.y - y);

        m_LastMousePosition.x = x;
        m_LastMousePosition.y = y;

        offset *= m_CameraSensivity;

        // TODO quaternion realization
        /*
        glm::vec3 angles = glm::eulerAngles(m_CameraRotation); //XYZ as pitch, yaw, and roll
        angles += glm::vec3(glm::radians(offset.y), glm::radians(offset.x), 0.0f);
        m_CameraRotation = glm::quat(glm::vec3(angles.x, angles.y, angles.z));
        m_Front = glm::normalize(glm::vec3(glm::cos(angles.y) * glm::cos(angles.x), glm::sin(angles.x), glm::sin(angles.y) * glm::cos(angles.x)));
        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));
        */

        // euler angles realization
        m_Yaw += glm::radians(offset.x);
        m_Pitch += glm::radians(offset.y);
        glm::vec3 angles = glm::eulerAngles(m_CameraRotation); //XYZ as pitch, yaw, and roll
        m_CameraRotation = glm::quat(glm::vec3(m_Pitch, m_Yaw, angles.z));

        m_Front = glm::normalize(glm::vec3(glm::cos(m_Yaw) * glm::cos(m_Pitch), glm::sin(m_Pitch), glm::sin(m_Yaw) * glm::cos(m_Pitch)));
        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));

        // Position
        float velocity = m_CameraSpeed * ts;
        if (Input::IsKeyPressed(HE_KEY_W)) // forawrd
        {
            m_CameraPosition += m_Front * velocity;
        }
        if (Input::IsKeyPressed(HE_KEY_S)) // backward
        {
            m_CameraPosition -= m_Front * velocity;
        }
        if (Input::IsKeyPressed(HE_KEY_D)) // dight
        {
            m_CameraPosition += m_Right * velocity;
        }
        if (Input::IsKeyPressed(HE_KEY_A)) // left
        {
            m_CameraPosition -= m_Right * velocity;
        }
        if (Input::IsKeyPressed(HE_KEY_Q)) // up
        {
            m_CameraPosition += m_Up * velocity;
        }
        if (Input::IsKeyPressed(HE_KEY_E)) // down
        {
            m_CameraPosition -= m_Up * velocity;
        }

        m_Camera->RecalculateViewMatrix(m_CameraPosition, m_Front, m_Up);
    }

    void PerspectiveCameraController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(HE_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScroll));
        dispatcher.Dispatch<WindowResizeEvent>(HE_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
    }

    bool PerspectiveCameraController::OnMouseScroll(MouseScrolledEvent& e)
    {
        // TODO кастомные настройки fov-a
        HE_CORE_INFO("{0}", e.GetYOffset());
        if (m_Fov >= 1.0f && m_Fov <= 45.0f)
            m_Fov += glm::radians(e.GetYOffset());
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
