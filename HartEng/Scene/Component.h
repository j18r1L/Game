#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "HartEng/Scene/Entity.h"

namespace HE
{
    class Entity;

    class Component
    {
    protected:
        Entity* m_EntityHandle = nullptr;
    public:

        virtual ~Component();

        void SetEntity(Entity* entity);
    };
}

#endif // COMPONENTS_H
