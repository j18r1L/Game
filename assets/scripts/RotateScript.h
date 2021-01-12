#ifndef ROTATESCRIPT_H
#define ROTATESCRIPT_H

#include "../HartEng/HartEng.h"

class RotateScript : public HE::ScriptComponent
{
private:
	float time = 0;
public:
	RotateScript(HE::Entity* entity):
		ScriptComponent(entity)
	{

	}

	void OnCreate() override
	{
		HE_INFO("RotateScript OnCreate");
		time = 0;
	}

	void OnUpdate(HE::Timestep& timestep) override
	{
		
		HE::TransformComponent* transformComponent = m_EntityHandle->GetComponent<HE::TransformComponent>();
		time += timestep * 2.0f;
		transformComponent->SetRotation({ 0.0f, 0.0f, time });
	}

	void OnDestroy() override
	{
		HE_INFO("RotateScript OnDestroy");
		HE::TransformComponent* transformComponent =m_EntityHandle->GetComponent<HE::TransformComponent>();
		transformComponent->SetRotation({ 0.0f, 0.0f, 0.0f });
	}
};

#endif // ROTATESCRIPT_H