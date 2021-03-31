#pragma once

#include "../HartEng/HartEng.h"

class FPSScript : public HE::ScriptComponent
{
private:
	float WalkingSpeed = 1.0F;
	float RunSpeed = 2.0F;
	float JumpForce = 0.1F;

	float MouseSensitivity = 10.0F;


	bool m_Colliding = false;
	float m_CurrentSpeed;

	std::shared_ptr<HE::PhysicsActor> m_PhysicsActor;
	HE::TransformComponent* m_Transform;
	HE::TransformComponent* m_CameraTransform;

	HE::Entity* m_CameraEntity;

	glm::vec2 m_LastMousePosition;

	float m_CurrentYMovement = 0.0F;

	glm::vec2 m_MovementDirection = { 0.0f, 0.0f };
	bool m_ShouldJump = false;
	bool m_CursorDisabled = false;
public:
	FPSScript(HE::Entity* entity);
	virtual ~FPSScript() = default;

	void OnCreate() override;
	void OnUpdate(HE::Timestep& ts) override;
	void OnPhysicsUpdate(float fixedTimeStep) override;

	void UpdateMovementInput();
	void UpdateRotation(float ts);
	void UpdateMovement();


	void OnCollisionBegin(const HE::Entity& entity) override;
	void OnCollisionEnd(const HE::Entity& entity) override;
	void OnTriggerBegin(const HE::Entity& entity) override;
	void OnTriggerEnd(const HE::Entity& entity) override;
};