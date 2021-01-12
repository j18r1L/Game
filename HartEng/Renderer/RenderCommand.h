#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H

#include "HartEng/Renderer/RendererAPI.h"

namespace HE
{
    // В отличии от Renderer является лишь интерфейсом к OpenGL, Vulkan, DirectX,
    // RenderCommand не должен выполнять никаких дополнительных действий,
    class RenderCommand
    {
    private:
        static RendererAPI* s_RendererAPI;
    public:
        inline static void Init()
        {
            s_RendererAPI->Init();
        }

        inline static void SetClearColor(const glm::vec4& color)
        {
            s_RendererAPI->SetClearColor(color);
        }

        inline static void SetViewport(uint32_t x, uint32_t y, uint32_t  width, uint32_t  height)
        {
            s_RendererAPI->SetViewport(x, y, width, height);
        }

        inline static void SetDepthTest(bool depthTest)
        {
            s_RendererAPI->SetDepthTest(depthTest);
        }

        inline static void Clear()
        {
            s_RendererAPI->Clear();
        }

        inline static void Blit(uint32_t srcx0, uint32_t srcy0, uint32_t srcx1, uint32_t srcy1, uint32_t dstx0, uint32_t dsty0, uint32_t dstx1, uint32_t dsty1)
        {
            s_RendererAPI->Blit(srcx0, srcy0, srcx1, srcy1, dstx0, dsty0, dstx1, dsty1);
        }

        inline static int ReadPixel(uint32_t x, uint32_t y)
        {
            return s_RendererAPI->ReadPixel(x, y);
        }

        inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
        {
            s_RendererAPI->DrawIndexed(vertexArray);
        }


    };
}
#endif // RENDERCOMMAND_H
