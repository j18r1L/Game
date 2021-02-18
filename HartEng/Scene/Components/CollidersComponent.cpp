#include "HartEng/Scene/Components/CollidersComponent.h"

namespace HE
{
	BoxColliderComponent::BoxColliderComponent(Entity* entityHandle)
	{
		m_EntityHandle = entityHandle;
	}

	RigidBodyComponent::RigidBodyComponent(Entity* entityHandle)
	{
		m_EntityHandle = entityHandle;
	}
}