
#include "HartEng/Renderer/VertexArray.h"
#include "HartEng/Renderer/Renderer.h"
#include "HartEng/Core/Core.h"
#include "HartEng/Core/Log.h"
#include "HartEng/Platform/OpenGL/OpenGLVertexArray.h"

namespace HE
{
    std::shared_ptr<VertexArray> VertexArray::Create()
    {
        RendererAPI::API api = Renderer::GetAPI();
        if (api == RendererAPI::API::OpenGL)
        {
            return std::shared_ptr<OpenGLVertexArray>(new OpenGLVertexArray());
        }
        else if  (api == RendererAPI::API::None)
        {
            HE_CORE_ASSERT(false, "RendererAPI::None currently not supported!");
            return nullptr;
        }
        HE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    
}
