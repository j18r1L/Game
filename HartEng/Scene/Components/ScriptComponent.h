#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

#include "HartEng/Scene/Component.h"

namespace HE
{
	class ScriptComponent: public Component
	{
	public:
		virtual void OnCreate() {};
		virtual void OnUpdate(Timestep& timestep) {};
		virtual void OnDestroy() {};
	};
}

#endif // SCRIPTCOMPONENT_H