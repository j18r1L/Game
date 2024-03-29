#ifndef OPENGLCONTEXT_H
#define OPENGLCONTEXT_H


#include "HartEng/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

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
