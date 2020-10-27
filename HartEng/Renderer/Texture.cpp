#include "HartEng/Renderer/Texture.h"

#include "HartEng/Core/Log.h"
#include "HartEng/Core/Core.h"

#include "HartEng/Renderer/Renderer.h"
#include "HartEng/Platform/OpenGL/OpenGLTexture.h"

namespace HE
{
    std::shared_ptr<Texture2D> Texture2D::Create(const std::string& filepath)
    {
        RendererAPI::API api = Renderer::GetAPI();
        if (api == RendererAPI::API::OpenGL)
        {
            return std::make_shared<OpenGLTexture2D>(filepath);
        }
        else if (api == RendererAPI::API::None)
        {
            HE_CORE_ASSERT(false, "RendererAPI::None currently not supported!");
            return nullptr;
        }
        HE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    std::shared_ptr<TextureCube> TextureCube::Create(const std::string& path)
    {
        RendererAPI::API api = Renderer::GetAPI();
        if (api == RendererAPI::API::OpenGL)
        {
            return std::make_shared<OpenGLTextureCube>(path);
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

