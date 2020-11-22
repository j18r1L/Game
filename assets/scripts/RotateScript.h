#ifndef ROTATESCRIPT_H
#define ROTATESCRIPT_H

#include "../HartEng/HartEng.h"

class RotateScript : public HE::ScriptComponent
{
private:
	float time = 0;
public:
	RotateScript(HE::Entity* entity)
	{
		m_EntityHandle = entity;
		m_Type = HE::ComponentType::ScriptComponent;
	}

	void OnCreate() override
	{
		HE_INFO("RotateScript OnCreate");
		time = 0;
	}

	void OnUpdate(HE::Timestep& timestep) override
	{
		
		HE::TransformComponent* transformComponent = dynamic_cast<HE::TransformComponent*>(m_EntityHandle->GetComponent(HE::ComponentType::TransformComponent));
		time += timestep * 2.0f;
		transformComponent->SetRotation({ 0.0f, 0.0f, time });
	}

	void OnDestroy() override
	{
		HE_INFO("RotateScript OnDestroy");
	}
};

#endif // ROTATESCRIPT_H