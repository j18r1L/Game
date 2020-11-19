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
        std::shared_ptr<VertexArray> m_VertexArray = nullptr;

        std::string m_Name = "Undefined SubMesh";

        std::shared_ptr<MaterialComponent> m_Material = nullptr; // TODO Can there be submesh without material?

        // Pointer to existing shader library and name of the shader
        // TODO do we need only shared_ptr to Shader?

    public:
        SubMeshComponent(Entity* entityHandle);
        virtual ~SubMeshComponent() = default;

        void SetAttribute(std::shared_ptr<VertexArray> vertexArray);
        void SetName(const std::string& name);
        void SetMaterial(const MaterialComponent& materialComponent);
        void SetMaterial(MaterialComponent* materialComponent);


        const std::shared_ptr<VertexArray>& GetAttribute() const;
        std::shared_ptr<MaterialComponent> GetMaterial() const;
        const std::string& GetName() const;

    };
}

#endif // SUBMESHCOMPONENT_H
