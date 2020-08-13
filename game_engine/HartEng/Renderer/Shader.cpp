#include "HartEng/Log.h"
#include "HartEng/Core.h"

#include "HartEng/Renderer/Renderer.h"
#include "HartEng/Renderer/Shader.h"
#include "HartEng/Platform/OpenGL/OpenGLShader.h"



namespace HE
{
    Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            HE_CORE_ASSERT(false, "RendererAPI::None currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLShader(vertexSrc, fragmentSrc);
        }
        HE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }



}

