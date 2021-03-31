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

		void Copy(Component* other_base) override {};
		virtual void OnCreate() {};
		virtual void OnUpdate(Timestep& timestep) {};
		virtual void OnPhysicsUpdate(float fixedTimestep) {};
		virtual void OnDestroy() {};
		virtual void OnCollisionBegin(const Entity& entity) {};
		virtual void OnCollisionEnd(const Entity& entity) {};
		virtual void OnTriggerBegin(const Entity& entity) {};
		virtual void OnTriggerEnd(const Entity& entity) {};
	};
}
