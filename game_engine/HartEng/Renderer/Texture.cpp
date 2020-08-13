#include "Texture.h"

#include "HartEng/Log.h"
#include "HartEng/Core.h"

#include "HartEng/Renderer/Renderer.h"
#include "HartEng/Platform/OpenGL/OpenGLTexture.h"

namespace HE
{
    std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            HE_CORE_ASSERT(false, "RendererAPI::None currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLTexture2D>(path);
        }
        HE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}

