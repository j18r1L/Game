#include "OpenGLContext.h"

namespace HE
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle):
        m_WindowHandle(windowHandle)
    {
        HE_CORE_ASSERT(windowHandle, "Window handle is NULL!");
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);
        //Initialize GLAD
        HE_CORE_INFO("Initializing GLAD...");
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        HE_CORE_ASSERT(status, "Failed to initialize GLAD!");

        HE_CORE_INFO("OpenGL Info:");
        HE_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        HE_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        HE_CORE_INFO("  Veesion: {0}", glGetString(GL_VERSION));



    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
}
