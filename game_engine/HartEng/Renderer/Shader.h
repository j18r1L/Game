#ifndef SHADER_H
#define SHADER_H

#include "HartEng/pch.h"
#include <glm/glm.hpp>

namespace HE
{
    class Shader
    {
    public:
        static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        //virtual void UploadVSUniformBuffer();
        //virtual void UploadVSRendererBuffer();
        //virtual void UploadVSMaterialBuffer();

        virtual void SetMat4(const std::string& name, const glm::mat4& value) const = 0;
        virtual void SetMat3(const std::string& name, const glm::mat3& value) const = 0;
        virtual void SetVec4(const std::string& name, const glm::vec4& value) const = 0;
        virtual void SetVec3(const std::string& name, const glm::vec3& value) const = 0;
        virtual void SetVec2(const std::string& name, const glm::vec2& value) const = 0;
        virtual void SetFloat(const std::string& name, float value) const = 0;
        virtual void SetInt(const std::string& name, int value) const = 0;
        virtual void SetBool(const std::string& name, bool value) const = 0;

    };
}

#endif // SHADER_H
