#ifndef OPENGLSHADER_H
#define OPENGLSHADER_H

#include "HartEng/Renderer/Shader.h"
#include <glm/glm.hpp>

namespace HE
{
    class OpenGLShader: public Shader
    {
    private:
        uint32_t m_RendererID;
    public:
        OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~OpenGLShader();

        void Bind() const override;
        void UnBind() const override;

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
