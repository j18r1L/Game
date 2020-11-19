#include "HartEng/Scene/Component.h"

namespace HE
{
    Component::~Component()
    {
    }

    ComponentType Component::getType() const
    {
        return m_Type;
    }

    void Component::SetEntity(Entity *entity)
    {
        m_EntityHandle = entity;
    }

    Component* Component::GetComponent(ComponentType componentType)
    {
        return m_EntityHandle->GetComponent(componentType);
    }
}
