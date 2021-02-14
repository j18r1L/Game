#pragma once

#include "HartEng/Scene/Component.h"

namespace HE
{
	class ScriptComponent: public Component
	{
	public:
		ScriptComponent(Entity* entityHandle)
		{
			m_EntityHandle = entityHandle;
		}
		virtual ~ScriptComponent() = default;
		virtual void OnCreate() {};
		virtual void OnUpdate(Timestep& timestep) {};
		virtual void OnDestroy() {};
		virtual void OnCollision(Entity* entity) {};
	};
}
