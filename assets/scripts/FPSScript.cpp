#include "FPSScript.h"


FPSScript::FPSScript(HE::Entity* entity):
	ScriptComponent(entity)
{
	WalkingSpeed = 6.0f;
	RunSpeed = 9.0f;
	JumpForce = 2.0f;
}

void FPSScript::OnCreate()
{
	m_Transform = m_EntityHandle->GetComponent<HE::TransformComponent>();
	m_PhysicsActor = HE::Physics::GetActorForEntity(*m_EntityHandle);
	
	m_CurrentSpeed = WalkingSpeed;
	
	m_CameraEntity = m_EntityHandle->GetScene()->FindEntityByTag("Camera");
	m_CameraTransform = m_CameraEntity->GetComponent<HE::TransformComponent>();


	 auto& [x, y] = HE::Input::GetMousePosition();
	 m_LastMousePosition = { x, y };


	HE::Application::Get().GetWindow().DisableCursor();
}

void FPSScript::OnUpdate(HE::Timestep& ts)
{
	if (HE::Input::IsKeyPressed(HE_KEY_ESCAPE))
	{
		HE::Application::Get().GetWindow().ShowCursor();
	}

	if (HE::Input::IsMouseButtonPressed(2))
	{
		HE::Application::Get().GetWindow().DisableCursor();
	}

	m_CurrentSpeed = HE::Input::IsKeyPressed(HE_KEY_LEFT_CONTROL) ? RunSpeed : WalkingSpeed;

	UpdateMovementInput();
	UpdateRotation(ts);
}

void FPSScript::UpdateMovementInput()
{
	if (HE::Input::IsKeyPressed(HE_KEY_W))
		m_MovementDirection.y = 1.0F;
	else if (HE::Input::IsKeyPressed(HE_KEY_S))
		m_MovementDirection.y = -1.0F;
	else
		m_MovementDirection.y = 0.0F;

	if (HE::Input::IsKeyPressed(HE_KEY_A))
		m_MovementDirection.x = -1.0F;
	else if (HE::Input::IsKeyPressed(HE_KEY_D))
		m_MovementDirection.x = 1.0F;
	else
		m_MovementDirection.x = 0.0F;

	m_ShouldJump = HE::Input::IsKeyPressed(HE_KEY_SPACE) && !m_ShouldJump;
}

void FPSScript::OnPhysicsUpdate(float fixedTimeStep)
{
	UpdateMovement();
}

void FPSScript::UpdateRotation(float ts)
{
	// TODO: Mouse position should be relative to the viewport
	auto& [x, y] = HE::Input::GetMousePosition();
	glm::vec2 currentMousePosition = { x, y };
	glm::vec2 delta = m_LastMousePosition - currentMousePosition;
	m_CurrentYMovement = delta.x * MouseSensitivity * ts;
	float xRotation = delta.y * MouseSensitivity * ts;

	if (m_CurrentYMovement != 0.0f || xRotation != 0.0f)
	{
		m_CameraTransform->SetRotation(glm::degrees(m_CameraTransform->GetRotation()) + glm::vec3(xRotation, 0.0f, m_CurrentYMovement));
	}

	m_CameraTransform->SetRotation(glm::vec3(glm::clamp(glm::degrees(m_CameraTransform->GetRotation().x), -80.0f, 80.0f), 0.0f, glm::degrees(m_CameraTransform->GetRotation().z)));
	m_LastMousePosition = currentMousePosition;
}

void FPSScript::UpdateMovement()
{
	auto& transform = m_CameraTransform->GetTransform();

	glm::vec3 right = normalize(glm::vec3(transform[0]));
	glm::vec3 forward = normalize(-glm::vec3(transform[2]));


	glm::vec3 movement = glm::normalize(right * m_MovementDirection.x + forward * m_MovementDirection.y);
	glm::vec3 velocity = movement * m_CurrentSpeed;
	velocity.y = m_PhysicsActor->GetLinearVelocity().y;
	m_PhysicsActor->SetLinearVelocity(velocity);

	if (m_ShouldJump && m_Colliding)
	{
		m_PhysicsActor->AddForce(glm::vec3(0.0f, 1.0f, 0.0f) * JumpForce, HE::ForceMode::Impulse);
		m_ShouldJump = false;
	}
}

void FPSScript::OnCollisionBegin(const HE::Entity& entity)
{
	m_Colliding = true;
}

void FPSScript::OnCollisionEnd(const HE::Entity& entity)
{
	m_Colliding = false;
}

void FPSScript::OnTriggerBegin(const HE::Entity& entity)
{

}

void FPSScript::OnTriggerEnd(const HE::Entity& entity)
{

}