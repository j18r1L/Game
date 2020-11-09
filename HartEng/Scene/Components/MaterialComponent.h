#ifndef MATERIALCOMPONENT_H
#define MATERIALCOMPONENT_H

#include "HartEng/Core/pch.h"

#include "glm/glm.hpp"
#include "HartEng/Scene/Components/Texture2DComponent.h"
#include "HartEng/Scene/Component.h"
#include "HartEng/Renderer/Shader.h"

namespace HE
{
    // В будущем будет содержать шейдер и название юниформ, дату для них
    class MaterialComponent: public Component
    {

    private:
        std::unordered_map<std::string, std::shared_ptr<Texture2DComponent>> m_Textures; // TODO do we need other textures (cubemap, 3d, ...)?

        // Emissive color of the material
        glm::vec3 m_Emissive = glm::vec3(0.0f);

        std::shared_ptr<ShaderLibrary> m_ShaderLibrary = nullptr;
        std::string m_ShaderName = "undefind shader";
    public:
        MaterialComponent();
        MaterialComponent(Entity* entityHandle);

        virtual ~MaterialComponent() = default;

        void AddTexture(const std::string& name, std::shared_ptr<Texture2DComponent> texture2DComponent);
        void AddTexture(const std::string& name, Texture2DComponent* texture2DComponent);
        void AddTexture(const std::string& name, const std::string filepath);

        void SetShader(std::shared_ptr<ShaderLibrary> shaderLibrary, const std::string& shaderName);

        const std::unordered_map<std::string, std::shared_ptr<Texture2DComponent>>& GetTextures() const;
        const glm::vec3& GetEmissive() const;
        const std::shared_ptr<ShaderLibrary>& GetShaderLibrary() const;
        const std::string& GetShaderName() const;
        std::shared_ptr<Shader> GetShader() const;
    };
}

#endif // MATERIALCOMPONENT_H
