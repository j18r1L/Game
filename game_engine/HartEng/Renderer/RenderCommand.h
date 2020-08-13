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

        inline static void SetClearColor(const glm::vec4& color)
        {
            s_RendererAPI->SetClearColor(color);
        }
        inline static void Clear()
        {
            s_RendererAPI->Clear();
        }

        inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
        {
            s_RendererAPI->DrawIndexed(vertexArray);
        }
    };
}
#endif // RENDERCOMMAND_H
