#pragma once

#include "HartEng/Renderer/RenderCommandQueue.h"
#include "HartEng/Renderer/RenderCommand.h"
#include "HartEng/Renderer/RenderPass.h"
#include "HartEng/Renderer/Material.h"
#include "HartEng/Renderer/Cameras.h"
#include "HartEng/Renderer/Shader.h"
#include "HartEng/Renderer/Mesh.h"
#include "HartEng/Scene/Components/LightComponent.h"

namespace HE
{
    // Renderer это то с чем работает клиент, этот класс  втом числе существует, чтобы скрыть имплементацию определенных действий
    // Например то, что нам нужно забиндить Vertex Array прежде чем вызывать Draw
    class Renderer
    {
    private:
        static RenderCommandQueue& GetRenderCommandQueue();

    public:
        // BeginSCene будет принимать environment map-у, uniform-ы, источники света, view, projection матрицы
        static void BeginRenderPass(std::shared_ptr<RenderPass> renderPass, bool clear = true);
        static void EndRenderPass();

        static void Init();
        static void Shutdown();

        static void WaitAndRender();

        static void OnWindowResize(uint32_t width, uint32_t height);

        static std::shared_ptr<ShaderLibrary> GetShaderLibrary();
        inline static RendererAPI::API GetAPI()
        {
            return RendererAPI::GetAPI();
        }

        static void SubmitMesh(std::shared_ptr<Mesh> mesh, const glm::mat4& transform, std::shared_ptr<MaterialInstance> overrideMaterial = nullptr);
        template<typename FuncT>
        static void Submit(FuncT&& func)
        {
            auto renderCmd = [](void* ptr) {
                auto pFunc = (FuncT*)ptr;
                (*pFunc)();

                // NOTE: Instead of destroying we could try and enforce all items to be trivally destructible
                // however some items like uniforms which contain std::strings still exist for now
                // static_assert(std::is_trivially_destructible_v<FuncT>, "FuncT must be trivially destructible");
                pFunc->~FuncT();
            };
            auto storageBuffer = GetRenderCommandQueue().Allocate(renderCmd, sizeof(func));
            new (storageBuffer) FuncT(std::forward<FuncT>(func));
        }
    };


}


