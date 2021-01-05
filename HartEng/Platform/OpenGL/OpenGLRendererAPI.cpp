#include "HartEng/Platform/OpenGL/OpenGLRendererAPI.h"

#include "HartEng/Core/pch.h"
#include <glad/glad.h>

namespace HE
{
    void OpenGLRendererAPI::Init()
    {
        HE_PROFILE_FUNCTION();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);

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

    void OpenGLRendererAPI::Blit(uint32_t srcx0, uint32_t srcy0, uint32_t srcx1, uint32_t srcy1, uint32_t dstx0, uint32_t dsty0, uint32_t dstx1, uint32_t dsty1)
    {
        HE_PROFILE_FUNCTION();

        glBlitFramebuffer(srcx0, srcy0, srcx1, srcy1, dstx0, dsty0, dstx1, dsty1, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }

    int OpenGLRendererAPI::ReadPixel(uint32_t x, uint32_t y)
    {
        // Read from ID buffer
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        int pixelData;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
        return pixelData;
    }

    void OpenGLRendererAPI::SetDepthTest(bool depthTest)
    {
        if (depthTest)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);

    }

    void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray)
    {
        HE_PROFILE_FUNCTION();

        // TODO получать флаги, что именно рисовать: GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_POINTS, ...
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
}
