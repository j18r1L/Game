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
        HE_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_WindowHandle);
        //Initialize GLAD
        HE_CORE_INFO("Initializing GLAD...");
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        HE_CORE_ASSERT(status, "Failed to initialize GLAD!");

        HE_CORE_INFO("OpenGL Info:");
        HE_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        HE_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        HE_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

        int major = 0;
        int minor = 0;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        HE_CORE_ASSERT(major > 4 || (major == 4 && minor >= 5), "OpenGL 4.5 is required, current OpenGL version is: " + std::to_string(major) + "." + std::to_string(minor));


    }

    void OpenGLContext::SwapBuffers()
    {
        HE_PROFILE_FUNCTION();

        glfwSwapBuffers(m_WindowHandle);
    }
}
