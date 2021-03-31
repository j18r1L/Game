#pragma once

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

        virtual void Copy(Component* other_base) = 0;

        void SetEntity(Entity* entity);
    };
}

