#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "HartEng/ECS/Entity.h"

namespace HE
{
    class Entity;
    enum class ComponentType
    {
        UndefinedComponent,
        TransformComponent,
        MaterialComponent,
        Texture2DComponent,
        MeshComponent,
        SubMeshComponent,
    };

    class Component
    {
    private:
        Component* GetComponent(ComponentType componentType);

    protected:
        Entity* m_EntityHandle = nullptr;
        ComponentType m_Type = ComponentType::UndefinedComponent;
    public:

        virtual ~Component();

        ComponentType getType() const;
    };
}

#endif // COMPONENTS_H
