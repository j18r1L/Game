#ifndef SHADER_H
#define SHADER_H

#include "HartEng/pch.h"
#include <glm/glm.hpp>

namespace HE
{
    class Shader
    {
    public:
        static std::shared_ptr<Shader> Create(const std::string& path);
        static std::shared_ptr<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        //static Shader* Create(const std::string& path);
        //static Shader* Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        virtual const std::string& GetName() const = 0;

        //virtual void UploadVSUniformBuffer();
        //virtual void UploadVSRendererBuffer();
        //virtual void UploadVSMaterialBuffer();

        //TODO временно пока не сделаю нормальную material system
        virtual void SetMat4(const std::string& name, const glm::mat4& value) const = 0;
        virtual void SetMat3(const std::string& name, const glm::mat3& value) const = 0;
        virtual void SetVec4(const std::string& name, const glm::vec4& value) const = 0;
        virtual void SetVec3(const std::string& name, const glm::vec3& value) const = 0;
        virtual void SetVec2(const std::string& name, const glm::vec2& value) const = 0;
        virtual void SetFloat(const std::string& name, float value) const = 0;
        virtual void SetInt(const std::string& name, int value) const = 0;
        virtual void SetBool(const std::string& name, bool value) const = 0;

    };

    class ShaderLibrary
    {
    private:
        std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
    public:
        void Add(const std::shared_ptr<Shader> shader);
        void Add(const std::string& name, const std::shared_ptr<Shader> shader);

        std::shared_ptr<Shader> Load(const std::string& path);
        std::shared_ptr<Shader> Load(const std::string& name, const std::string& path);

        std::shared_ptr<Shader> Get(const std::string& name);

        bool Exists(const std::string& name) const;

    };
}

#endif // SHADER_H
