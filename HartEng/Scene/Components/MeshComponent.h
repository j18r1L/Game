#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "HartEng/Scene/Components/SubMeshComponent.h"

namespace HE
{
    class MeshComponent: public Component
    {
    private:
        std::vector<SubMeshComponent*> m_SubMeshes;
        // AABB m_BoundingVolume;
        // std::vector<Node*> m_Nodes;

    public:
        MeshComponent(Entity* entityHandle);
        virtual ~MeshComponent() = default;

        void AddSubMesh(SubMeshComponent& subMesh);

        const std::vector<SubMeshComponent*>& GetSubMeshes() const;
        //std::vector<SubMeshComponent*>& GetSubMeshes();

    };
}

#endif // MESHCOMPONENT_H
