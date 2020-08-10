#ifndef RENDERERAPI_H
#define RENDERERAPI_H

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
        inline static API GetAPI()
        {
            return s_API;
        }

        // Отчистить экран
        virtual void Clear() = 0;
        // Установить цвет отчистки экрана
        virtual void SetClearColor(const glm::vec4& color) = 0;

        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;
    private:
        static API s_API;


    };
}

#endif // RENDERERAPI_H
