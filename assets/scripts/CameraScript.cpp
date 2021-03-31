#include "CameraScript.h"

void CameraScript::OnCreate()
{
	m_PlayerTransformComponent = m_EntityHandle->GetScene()->FindEntityByTag("Player")->GetComponent<HE::TransformComponent>();
}

void  CameraScript::OnUpdate(HE::Timestep& timestep)
{
	HE::TransformComponent* transformComponent = m_EntityHandle->GetComponent<HE::TransformComponent>();
	//auto playerRotation = glm::degrees(m_PlayerTransformComponent->GetRotation());
	transformComponent->SetPosition(m_PlayerTransformComponent->GetPosition() + glm::vec3(0.0f, 0.4f, 0.0f)); // cube size = 1m
	//transformComponent->SetRotation(glm::degrees(m_PlayerTransformComponent->GetRotation()));
}

void  CameraScript::OnDestroy()
{

}