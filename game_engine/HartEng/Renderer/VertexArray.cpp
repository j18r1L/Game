
#include "HartEng/Renderer/VertexArray.h"
#include "HartEng/Renderer/Renderer.h"
#include "HartEng/Core.h"
#include "HartEng/Log.h"
#include "HartEng/Platform/OpenGL/OpenGLVertexArray.h"

namespace HE
{
    VertexArray* VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            HE_CORE_ASSERT(false, "RendererAPI::None currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLVertexArray();

        }

        HE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    
}
