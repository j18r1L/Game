#include "HartEng/Scene/Components/Circle2DColliderComponent.h"

namespace HE
{
	Circle2DColliderComponent::Circle2DColliderComponent(Entity* entityHandle)
	{
		m_EntityHandle = entityHandle;
	}

	bool Circle2DColliderComponent::Collision(const Circle2DColliderComponent& circle)
	{
		const glm::vec2 origin = circle.GetOrigin();
		float dx = m_Origin.x - origin.x;
		float dy = m_Origin.y - origin.y;
		float distance = glm::sqrt(dx * dx + dy * dy);

		if (distance < m_Radius + circle.GetRadius()) 
		{
			return true;
		}
		return false;
	}
}