#ifndef SHADER_H
#define SHADER_H

#include "HartEng/pch.h"
#include "HartEng/Log.h"
#include <glm/glm.hpp>

namespace HE
{
    class Shader
    {
    private:
        uint32_t m_RendererID;
    public:
        Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~Shader();

        void Bind() const;
        void UnBind() const;

        void SetMat4(const std::string& name, const glm::mat4& value);
        void SetMat3(const std::string& name, const glm::mat3& value);
        void SetVec4(const std::string& name, const glm::vec4& value);
        void SetVec3(const std::string& name, const glm::vec3& value);
        void SetVec2(const std::string& name, const glm::vec2& value);
        void SetFloat(const std::string& name, float value);
        void SetInt(const std::string& name, int value);
        void SetBool(const std::string& name, bool value);

    };
}

#endif // SHADER_H
