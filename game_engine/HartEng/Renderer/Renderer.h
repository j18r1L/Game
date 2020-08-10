#ifndef RENDERER_H
#define RENDERER_H

#include "HartEng/pch.h"
#include "HartEng/Renderer/RenderCommand.h"

namespace HE
{
    // Renderer это то с чем работает клиент, этот класс  втом числе существует, чтобы скрыть имплементацию определенных действий
    // Например то, что нам нужно забиндить Vertex Array прежде чем вызывать Draw
    class Renderer
    {
    public:
        // BeginSCene будет принимать environment map-у, uniform-ы, источники света, view, projection матрицы
        static void BeginScene();
        static void EndScene();

        static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

        inline static RendererAPI::API GetAPI()
        {
            return RendererAPI::GetAPI();
        }
    };


}

#endif // RENDERER_H

