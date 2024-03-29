#include "HartEng/Platform/OpenGL/OpenGLContext.h"
#include "HartEng/Core/pch.h"
#include "HartEng/Core/Core.h"
#include "HartEng/Core/Log.h"

#include <glad/glad.h>

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
        //int status =  gladLoadGL();
        HE_CORE_ASSERT(status, "Failed to initialize GLAD!");

        HE_CORE_INFO("OpenGL Info:");
        HE_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        HE_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        HE_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

        int major = 0;
        int minor = 0;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        HE_CORE_ASSERT(major > 4 || (major == 4 && minor >= 1), "OpenGL 4.1 is required, current OpenGL version is: " + std::to_string(major) + "." + std::to_string(minor));


    }

    void OpenGLContext::SwapBuffers()
    {
        HE_PROFILE_FUNCTION();

        glfwSwapBuffers(m_WindowHandle);
    }
}
