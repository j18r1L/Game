#ifndef SUBMESHCOMPONENT_H
#define SUBMESHCOMPONENT_H

#include "HartEng/Core/pch.h"

#include "HartEng/Scene/Component.h"
#include "HartEng/Scene/Components/MaterialComponent.h"
#include "HartEng/Renderer/VertexArray.h"
#include "HartEng/Renderer/Shader.h"

namespace HE
{
    class SubMeshComponent: public Component
    {
    private:
        // Vertex Arrays
        std::unordered_map<std::string, std::shared_ptr<VertexArray>> m_VertexArrays;

        std::shared_ptr<MaterialComponent> m_Material = nullptr; // TODO Can there be submesh without material?

        // Pointer to existing shader library and name of the shader
        // TODO do we need only shared_ptr to Shader?
        std::shared_ptr<ShaderLibrary> m_ShaderLibrary = nullptr;
        std::string m_ShaderName = "undefind shader";
    public:
        SubMeshComponent(Entity* entityHandle);
        SubMeshComponent(Entity* entityHandle, std::shared_ptr<MaterialComponent> materialComponent, std::shared_ptr<ShaderLibrary> shaderLibrary, const std::string& shaderName);
        virtual ~SubMeshComponent() = default;

        void SetAttribute(const std::string& name, std::shared_ptr<VertexArray> vertexArray);
        void SetMaterial(const MaterialComponent& materialComponent);
        void SetShader(std::shared_ptr<ShaderLibrary> shaderLibrary, const std::string& shaderName);

        const std::unordered_map<std::string, std::shared_ptr<VertexArray>>& GetAttribute() const;
        std::shared_ptr<MaterialComponent> GetMaterial() const;
        const std::shared_ptr<ShaderLibrary>& GetShaderLibrary() const;
        const std::string& GetShaderName() const;
        std::shared_ptr<Shader> GetShader() const;
    };
}

#endif // SUBMESHCOMPONENT_H
