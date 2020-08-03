#include "HartEng/Renderer/Buffer.h"

namespace HE
{
    VertexBuffer* VertexBuffer::Create(float *vertices, uint32_t size)
    {
        switch (Renderer::GetRendererAPI())
        {
        case RendererAPI::None:
            HE_CORE_ASSERT(false, "RendererAPI::None currently not supported!");
            return nullptr;
            break;
        case RendererAPI::OpenGL:
            return new OpenGLVertexBuffer(vertices, size);

        }

        HE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create(uint32_t *indices, uint32_t size)
    {
        switch (Renderer::GetRendererAPI())
        {
        case RendererAPI::None:
            HE_CORE_ASSERT(false, "RendererAPI::None currently not supported!");
            return nullptr;
            break;
        case RendererAPI::OpenGL:
            return new OpenGLIndexBuffer(indices, size);

        }

        HE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
