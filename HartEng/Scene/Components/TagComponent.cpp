#include "HartEng/Scene/Components/TagComponent.h"

namespace HE
{
	TagComponent::TagComponent(Entity* entityHandle)
	{
		m_EntityHandle = entityHandle;
	}

    void TagComponent::Copy(Component* other_base)
    {
		TagComponent* other = dynamic_cast<TagComponent*>(other_base);
        m_Tag = other->m_Tag;
    }
}