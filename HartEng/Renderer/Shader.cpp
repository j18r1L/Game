#include "HartEng/Core/Log.h"
#include "HartEng/Core/Core.h"

#include "HartEng/Renderer/Renderer.h"
#include "HartEng/Renderer/Shader.h"
#include "HartEng/Platform/OpenGL/OpenGLShader.h"



namespace HE
{
    std::shared_ptr<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    {

        RendererAPI::API api = Renderer::GetAPI();
        if (api == RendererAPI::API::OpenGL)
        {
            return std::shared_ptr<OpenGLShader> (new OpenGLShader(name, vertexSrc, fragmentSrc));
        }
        else if (api == RendererAPI::API::None)
        {
            HE_CORE_ASSERT(false, "RendererAPI::None currently not supported!");
            return nullptr;
        }
        HE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    std::shared_ptr<Shader> Shader::Create(const std::string& path)
    {
        RendererAPI::API api = Renderer::GetAPI();
        if (api == RendererAPI::API::OpenGL)
        {
            return std::shared_ptr<Shader> (new OpenGLShader(path));
        }
        else if (api == RendererAPI::API::None)
        {
            HE_CORE_ASSERT(false, "RendererAPI::None currently not supported!");
            return nullptr;
        }
        HE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    void ShaderLibrary::Add(const std::string& name, const std::shared_ptr<Shader> shader)
    {
        HE_CORE_ASSERT(!Exists(name), "Shader already exists!");
        m_Shaders[name] = shader;
    }

    void ShaderLibrary::Add(const std::shared_ptr<Shader> shader)
    {
        auto& name = shader->GetName();
        Add(name, shader);
    }



    std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& path)
    {
        auto shader = Shader::Create(path);
        Add(shader);
        return shader;
    }

    std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& name, const std::string& path)
    {
        auto shader = Shader::Create(path);
        Add(name, shader);
        return shader;
    }

    std::shared_ptr<Shader> ShaderLibrary::Get(const std::string& name)
    {
        HE_CORE_ASSERT(Exists(name), "Shader not found!");
        return m_Shaders[name];
    }

    const std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderLibrary::GetShaders() const
    {
        return m_Shaders;
    }

    bool ShaderLibrary::Exists(const std::string& name) const
    {
        return m_Shaders.find(name) != m_Shaders.end();
    }



}

