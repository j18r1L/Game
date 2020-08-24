#include "HartEng/Platform/OpenGL/OpenGLRendererAPI.h"

#include "HartEng/pch.h"

namespace HE
{
    void OpenGLRendererAPI::Init()
    {
        HE_PROFILE_FUNCTION();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4 &color)
    {
        HE_PROFILE_FUNCTION();

        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t  width, uint32_t  height)
    {
        HE_PROFILE_FUNCTION();

        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::Clear()
    {
        HE_PROFILE_FUNCTION();

        // TODO получать на вход флаги что именно отчистить (в будущем framebuffer-ы мб буду заниматься отчисткой)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray)
    {
        HE_PROFILE_FUNCTION();

        // TODO получать флаги, что именно рисовать: GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_POINTS, ...
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
}
