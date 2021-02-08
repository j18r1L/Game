#pragma once

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
        static void Init();

        static void SetClearColor(const glm::vec4& color);

        static void SetViewport(uint32_t x, uint32_t y, uint32_t  width, uint32_t  height);

        static void SetDepthTest(bool depthTest);

        static void Clear();

        static void Blit(uint32_t srcx0, uint32_t srcy0, uint32_t srcx1, uint32_t srcy1, uint32_t dstx0, uint32_t dsty0, uint32_t dstx1, uint32_t dsty1);

        static int ReadPixel(uint32_t x, uint32_t y);

        static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);
        static void DrawIndexed(uint32_t indexCount);


    };
}
