#include "HartEng/Scene/Component.h"

namespace HE
{
    Component::~Component()
    {
        m_EntityHandle->RemoveComponent(m_Type);
    }

    ComponentType Component::getType() const
    {
        return m_Type;
    }

    Component* Component::GetComponent(ComponentType componentType)
    {
        return m_EntityHandle->GetComponent(componentType);
    }
}
