#include "LinuxInput.h"

namespace HE
{
    // TODO может быть переместить это в другое место
    Input* Input::s_Instance = new LinuxInput();


    bool LinuxInput::IsKeyPressedImpl(int keycode)
    {
        // Берем указатель на экран
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

        // Проверяем нажата ли клавиша
        auto state = glfwGetKey(window, keycode);
        return ((state == GLFW_PRESS) || (state == GLFW_REPEAT));
    }

    bool LinuxInput::IsMouseButtonPressedImpl(int button)
    {
        // Берем указатель на экран
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

        auto state = glfwGetMouseButton(window, button);
        return (state == GLFW_PRESS);
    }
    float LinuxInput::GetMouseXImpl()
    {
        // Берем указатель на экран
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return (float)xpos;

    }
    float LinuxInput::GetMouseYImpl()
    {
        // Берем указатель на экран
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return (float)ypos;
    }

    std::pair<float, float> LinuxInput::GetMousePositionImpl()
    {
        // Берем указатель на экран
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return {xpos, ypos};
    }
}
