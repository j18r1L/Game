#pragma once

#include "../HartEng/HartEng.h"

class CameraScript : public HE::ScriptComponent
{
private:
	HE::TransformComponent* m_PlayerTransformComponent;
public:
	CameraScript(HE::Entity* entity) :
		ScriptComponent(entity)
	{

	}

	void OnCreate() override;

	void OnUpdate(HE::Timestep& timestep) override;

	void OnDestroy() override;
};