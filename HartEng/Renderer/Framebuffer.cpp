#include "HartEng/Renderer/Framebuffer.h"
#include "HartEng/Renderer/Renderer.h"
#include "HartEng/Platform/OpenGL/OpenGLFrameBuffer.h"

namespace HE
{
    std::shared_ptr<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification &spec)
    {
        RendererAPI::API api = Renderer::GetAPI();
        if (api == RendererAPI::API::OpenGL)
        {
            return std::shared_ptr<OpenGLFrameBuffer>(new OpenGLFrameBuffer(spec));
        }
        else if (api == RendererAPI::API::None)
        {
            HE_CORE_ASSERT(false, "RendererAPI::None currently not supported!");
            return nullptr;
        }
        HE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;

    }
}

