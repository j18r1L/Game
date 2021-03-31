

#include "HartEng/Core/pch.h"
#include "HartEng/Renderer/Renderer.h"
#include "HartEng/Renderer/RenderCommand.h"
#include "HartEng/Platform/OpenGL/OpenGLRendererAPI.h"

namespace HE
{
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

    void RenderCommand::Init()
    {
        s_RendererAPI->Init();
    }

    void RenderCommand::SetClearColor(const glm::vec4& color)
    {
        s_RendererAPI->SetClearColor(color);
    }

    void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t  width, uint32_t  height)
    {
        s_RendererAPI->SetViewport(x, y, width, height);
    }

    void RenderCommand::SetDepthTest(bool depthTest)
    {
        s_RendererAPI->SetDepthTest(depthTest);
    }

    void RenderCommand::Clear()
    {
        s_RendererAPI->Clear();

    }

    void RenderCommand::Blit(uint32_t srcx0, uint32_t srcy0, uint32_t srcx1, uint32_t srcy1, uint32_t dstx0, uint32_t dsty0, uint32_t dstx1, uint32_t dsty1)
    {
        s_RendererAPI->Blit(srcx0, srcy0, srcx1, srcy1, dstx0, dsty0, dstx1, dsty1);
    }

    int RenderCommand::ReadPixel(uint32_t x, uint32_t y)
    {
        return s_RendererAPI->ReadPixel(x, y);
    }

    void RenderCommand::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
    {
         s_RendererAPI->DrawIndexed(vertexArray);
    }

    void RenderCommand::DrawIndexed(uint32_t indexCount)
    {
        s_RendererAPI->DrawIndexed(indexCount);
    }
}
