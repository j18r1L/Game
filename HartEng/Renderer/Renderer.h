#pragma once

#include "HartEng/Renderer/RenderCommand.h"
#include "HartEng/Renderer/RenderPass.h"
#include "HartEng/Renderer/Cameras.h"
#include "HartEng/Renderer/Shader.h"
#include "HartEng/Scene/Components/MaterialComponent.h"
#include "HartEng/Scene/Components/LightComponent.h"

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
            std::shared_ptr<RenderPass> m_ActiveRenderPass;

            glm::mat4 ProjectionView;
            glm::mat4 View;

            std::vector<Entity*> Lights;
        };

        static SceneData* m_SceneData;
    public:
        // BeginSCene будет принимать environment map-у, uniform-ы, источники света, view, projection матрицы
        static void BeginRenderPass(std::shared_ptr<RenderPass> renderPass, bool clear = true);
        static void EndRenderPass();

        static void BeginScene(PerspectiveCamera& camera, const std::vector<Entity*>& lights); // This used only in level editor as non-runtime camera
        static void BeginScene(const glm::mat4& projection000, const glm::mat4 transform, const std::vector<Entity*>& lights);
        static void EndScene();

        static void Init();
        static void Shutdown();

        static void OnWindowResize(uint32_t width, uint32_t height);
        
        static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
        static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, uint32_t entityID, const glm::mat4& transform = glm::mat4(1.0f));
        static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform, std::shared_ptr<MaterialComponent> material);

        inline static RendererAPI::API GetAPI()
        {
            return RendererAPI::GetAPI();
        }
    };


}


