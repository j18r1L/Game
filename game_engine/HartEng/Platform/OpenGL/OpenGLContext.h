#ifndef OPENGLCONTEXT_H
#define OPENGLCONTEXT_H

#include "HartEng/pch.h"
#include "HartEng/Core.h"
#include "HartEng/Log.h"
#include "HartEng/Renderer/GraphicsContext.h"


//struct GLFWwindow;
namespace HE
{
    class OpenGLContext: public GraphicsContext
    {
    private:
        GLFWwindow* m_WindowHandle;
    public:
        OpenGLContext(GLFWwindow* windowHandle);
        void Init() override;
        void SwapBuffers() override;
    };
}

#endif // OPENGLCONTEXT_H
