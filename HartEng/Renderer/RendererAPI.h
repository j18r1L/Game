#pragma once

#include "glm/glm.hpp"

#include "HartEng/Renderer/VertexArray.h"

namespace HE
{
    class RendererAPI
    {
    public:
        enum class API
        {
            None = 0, OpenGL = 1
        };

    public:
        virtual ~RendererAPI() = default;
        inline static API GetAPI()
        {
            return s_API;
        }

        virtual void Init() = 0;
        // Отчистить экран
        virtual void Clear() = 0;
        // Blit
        virtual void Blit(uint32_t srcx0, uint32_t srcy0, uint32_t srcx1, uint32_t srcy1, uint32_t dstx0, uint32_t dsty0, uint32_t dstx1, uint32_t dsty1) = 0;
        //ReadPixels
        virtual int ReadPixel(uint32_t x, uint32_t y) = 0;
        // Установить цвет отчистки экрана
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t  width, uint32_t  height) = 0;
        virtual void SetDepthTest(bool depthTest) = 0;

        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;
        virtual void DrawIndexed(uint32_t indexCount) = 0;
    private:
        static API s_API;


    };
}

