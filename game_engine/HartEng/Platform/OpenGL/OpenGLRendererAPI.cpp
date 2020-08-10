#include "HartEng/Platform/OpenGL/OpenGLRendererAPI.h"

#include "HartEng/pch.h"

namespace HE
{
    void OpenGLRendererAPI::SetClearColor(const glm::vec4 &color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear()
    {
        // TODO получать на вход флаги что именно отчистить (в будущем framebuffer-ы мб буду заниматься отчисткой)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray)
    {
        // TODO получать флаги, что именно рисовать: GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_POINTS, ...
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
}
