#include "HartEng/Scene/Component.h"

namespace HE
{
    Component::~Component()
    {
    }

    void Component::SetEntity(Entity *entity)
    {
        m_EntityHandle = entity;
    }

}
