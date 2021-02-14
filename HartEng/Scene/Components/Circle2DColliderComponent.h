#pragma once

#include "HartEng/Scene/Component.h"


namespace HE
{
	class Circle2DColliderComponent : public Component
	{
	private:
		glm::vec2 m_Origin{0.0f};
		float m_Radius = 0.0f;
	public:
		Circle2DColliderComponent(Entity* entityHandle);
		virtual ~Circle2DColliderComponent() = default;

		void SetRadius(float radius) { m_Radius = radius; }
		void SetOrigin(const glm::vec2& origin) { m_Origin = origin; }

		float GetRadius() const { return m_Radius; }
		const glm::vec2& GetOrigin() const { return m_Origin; }

		bool Collision(const Circle2DColliderComponent& circle);


	};
}