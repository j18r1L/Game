#include "HartEng/Renderer/Buffer.h"
#include "HartEng/Renderer/Renderer.h"
#include "HartEng/Core.h"
#include "HartEng/Log.h"

#include "HartEng/Platform/OpenGL/OpenGLBuffer.h"

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
            break;

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
            break;

        }

        HE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
