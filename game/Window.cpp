#include "Window.h"

Window::Window(GLint width, GLint height, bool full_screen)
{
    if (full_screen)
        m_Window = glfwCreateWindow(width, height, "game", glfwGetPrimaryMonitor(), NULL);
    else
        m_Window = glfwCreateWindow(width, height, "game", NULL, NULL);
    if (m_Window == NULL)
    {
        glfwTerminate();
    }
}

void Window::MakeContextCurrent() const
{
    glfwMakeContextCurrent(m_Window);
}

void Window::setKeyCallback(GLFWkeyfun key_callback)
{
     glfwSetKeyCallback(m_Window, key_callback);
}


void Window::setCursorPosCallback(GLFWcursorposfun mouse_callback)
{
    glfwSetCursorPosCallback(m_Window, mouse_callback);
}

void Window::setScrollCallback(GLFWscrollfun scroll_callback)
{
    glfwSetScrollCallback(m_Window, scroll_callback);
}


