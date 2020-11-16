#ifndef OPENGLSHADER_H
#define OPENGLSHADER_H

#include "HartEng/Core/pch.h"
#include "HartEng/Renderer/Shader.h"
#include <glm/glm.hpp>

#include <glad/glad.h>

namespace HE
{
    class OpenGLShader: public Shader
    {
    private:
        uint32_t m_RendererID;
        std::string m_Name;
        std::string m_FilePath;

        mutable std::unordered_map<std::string, uint32_t> m_UniformLocationCache;



        std::string ReadFile(const std::string& path);
        void Compile(std::unordered_map<GLenum, std::string>& shaderSources);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& shaderSrc);
        uint32_t GetUniformLocation(const std::string& name) const;
    public:
        OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        OpenGLShader(const std::string& path);
        virtual ~OpenGLShader();

        void Bind() const override;
        void UnBind() const override;

        const std::string& GetName() const override;
        const std::string& GetFilePath() const override;

        void SetMat4(const std::string& name, const glm::mat4& value) const override;
        void SetMat3(const std::string& name, const glm::mat3& value) const override;
        void SetVec4(const std::string& name, const glm::vec4& value) const override;
        void SetVec3(const std::string& name, const glm::vec3& value) const override;
        void SetVec2(const std::string& name, const glm::vec2& value) const override;
        void SetFloat(const std::string& name, float value) const override;
        void SetInt(const std::string& name, int value) const override;
        void SetBool(const std::string& name, bool value) const override;

    };
}

#endif // OPENGLSHADER_H
