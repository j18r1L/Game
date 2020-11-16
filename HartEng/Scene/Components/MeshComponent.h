#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "HartEng/Scene/Components/SubMeshComponent.h"

namespace HE
{
    class MeshComponent: public Component
    {
    private:
        std::vector<SubMeshComponent*> m_SubMeshes;
        std::string m_Path;
        // AABB m_BoundingVolume;
        // std::vector<Node*> m_Nodes;

    public:
        MeshComponent(Entity* entityHandle);
        virtual ~MeshComponent() = default;

        void AddSubMesh(SubMeshComponent& subMesh);
        void SetPath(const std::string& path);
        const std::string& GetPath();

        const std::vector<SubMeshComponent*>& GetSubMeshes() const;

    };
}

#endif // MESHCOMPONENT_H
