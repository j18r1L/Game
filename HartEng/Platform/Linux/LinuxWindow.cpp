#include "HartEng/Platform/Linux/LinuxWindow.h"
#include "HartEng/Core/Core.h"
#include "HartEng/Core/Input.h"

#include "HartEng/Core/Keycodes.h"
#include "HartEng/Core/MouseButtonCodes.h"

#include "HartEng/Platform/OpenGL/OpenGLContext.h"
#include "HartEng/Events/MousesEvent.h"
#include "HartEng/Events/KeyEvent.h"
#include "HartEng/Events/ApplicationEvent.h"
#include "HartEng/Renderer/RenderCommand.h"
#ifdef HE_API_OPENGL
    #include <glad/glad.h>
#endif
// GLFW platform specific code
namespace HE
{
    static uint8_t s_GLFWWindowCount = 0;

    Window* Window::Create(const WindowProps& props)
    {
        return new LinuxWindow(props);
    }

    LinuxWindow::LinuxWindow(const WindowProps& props)
    {
        HE_PROFILE_FUNCTION();

        Init(props);
    }

    LinuxWindow::~LinuxWindow()
    {
        HE_PROFILE_FUNCTION();
        Shutdown();
    }
    void LinuxWindow::Init(const WindowProps& props)
    {
        HE_PROFILE_FUNCTION();

        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;
        m_Data.XPosition = props.XPosition;
        m_Data.YPosition = props.YPosition;


        HE_CORE_INFO("Creating window: {0} ({1}, {2})", props.Title, props.Width, props.Height);
        HE_CORE_INFO("Window position: {0}, {1}", props.XPosition, props.YPosition);


        if (s_GLFWWindowCount == 0)
        {
            int success = glfwInit();
            HE_CORE_ASSERT(success, "Could not initialize GLFW!");
#ifdef HE_API_OPENGL
            // TODO ради поддержки macos необходимо явно указать версию OpenGL-a
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

            // set error callback for glfw
            glfwSetErrorCallback(GLFWErrorCallback);
        }
        {
            HE_PROFILE_SCOPE("glfwCreateWindow");
            m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
            ++s_GLFWWindowCount;

        }
        // TODO сделать GraphicsContext::Create(m_Window);
        m_Context = new OpenGLContext(m_Window);
        m_Context->Init();
        glfwSetWindowUserPointer(m_Window, &m_Data);
        glfwSetWindowPos(m_Window, props.XPosition, props.YPosition);



        // TODO set given vsync
        SetVSync(true);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            WindowResizeEvent event(width, height);
            data.Width = width;
            data.Height = height;
            data.EventCallback(event);
        });

        glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.XPosition = xpos;
            data.YPosition = ypos;
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });


        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            switch(action)
            {
            case HE_PRESS:
            {
                KeyPressedEvent event(key, 0);
                data.EventCallback(event);
                break;
            }
            case HE_RELEASE:
            {
                KeyReleasedEvent event(key);
                data.EventCallback(event);
                break;
            }
            case HE_REPEAT:
            {
                KeyPressedEvent event(key, 1);
                data.EventCallback(event);
                break;
            }
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            KeyTypedEvent event(keycode);
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
             WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

             switch (action)
             {
             case HE_PRESS:
             {
                 MouseButtonPressedEvent event(button);
                 data.EventCallback(event);
                 break;
             }
             case HE_RELEASE:
             {
                 MouseButtonReleasedEvent event(button);
                 data.EventCallback(event);
                 break;
             }
             }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)xpos, (float)ypos);
            data.EventCallback(event);
        });


    }

    double LinuxWindow::GetTime() const
    {
        return glfwGetTime();
    }

    void LinuxWindow::Shutdown()
    {
        HE_PROFILE_FUNCTION();
        glfwDestroyWindow(m_Window);

        --s_GLFWWindowCount;
        if (s_GLFWWindowCount == 0)
        {
            glfwTerminate();
        }
    }

    void LinuxWindow::OnUpdate()
    {
        HE_PROFILE_FUNCTION();
        glfwPollEvents();
        m_Context->SwapBuffers();
        HE::RenderCommand::Clear();

    }

    void LinuxWindow::DisableCursor()
    {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void LinuxWindow::HideCursor()
    {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

    void LinuxWindow::ShowCursor()
    {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void LinuxWindow::SetVSync(bool enabled)
    {
        HE_PROFILE_FUNCTION();
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
    }

    bool LinuxWindow::IsSync() const
    {
        return m_Data.VSync;
    }

    // Callback functions
    static void GLFWErrorCallback(int error, const char* description)
    {
        HE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }


}
