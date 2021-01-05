#ifndef OPENGLRENDERERAPI_H
#define OPENGLRENDERERAPI_H

#include "HartEng/Renderer/RendererAPI.h"

namespace HE
{
    class OpenGLRendererAPI: public RendererAPI
    {
    private:

    public:
        void Init() override;
        void Clear() override;
        void Blit(uint32_t srcx0, uint32_t srcy0, uint32_t srcx1, uint32_t srcy1, uint32_t dstx0, uint32_t dsty0, uint32_t dstx1, uint32_t dsty1) override;
        int ReadPixel(uint32_t x, uint32_t y) override;
        void SetClearColor(const glm::vec4 &color) override;
        void SetViewport(uint32_t x, uint32_t y, uint32_t  width, uint32_t  height) override;
        void SetDepthTest(bool depthTest) override;

        void DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray) override;

    };
}

#endif // OPENGLRENDERERAPI_H
