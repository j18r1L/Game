#pragma once

#include "HartEng/Scene/Component.h"
#include "HartEng/Renderer/Mesh.h"

namespace HE
{
    class MeshComponent: public Component
    {
    private:
        std::shared_ptr<Mesh> m_Mesh;

    public:
        MeshComponent(Entity* entityHandle);
        virtual ~MeshComponent() = default;

        void Copy(Component* other_base) override;

        void SetMesh(const std::shared_ptr<Mesh>& mesh);
        std::shared_ptr<Mesh> GetMesh();

    };
}

