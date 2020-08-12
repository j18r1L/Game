#ifndef RENDERER_H
#define RENDERER_H

#include "HartEng/Renderer/RenderCommand.h"
#include "HartEng/Renderer/Cameras.h"
#include "HartEng/Renderer/Shader.h"

namespace HE
{
    // Renderer это то с чем работает клиент, этот класс  втом числе существует, чтобы скрыть имплементацию определенных действий
    // Например то, что нам нужно забиндить Vertex Array прежде чем вызывать Draw
    class Renderer
    {
    private:
        // TODO временно, нужно добавить que и буффер для хранения state-ов
        struct SceneData
        {
            glm::mat4 ProjectionViewMatrix;
        };

        static SceneData* m_SceneData;
    public:
        // BeginSCene будет принимать environment map-у, uniform-ы, источники света, view, projection матрицы
        static void BeginScene(OrthographicCamera& camera);
        static void EndScene();

        static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

        inline static RendererAPI::API GetAPI()
        {
            return RendererAPI::GetAPI();
        }
    };


}

#endif // RENDERER_H

