#pragma once

#include "../HartEng/HartEng.h"

#include "BulletScript.h"

class PlayerControllerScript : public HE::ScriptComponent
{
private:
	float m_VerticalSpeed = 3.7f;
	float m_HorizontalSpeed = 3.5f;
	bool m_Updated = false;

	float m_BulletTime = 250.0f; // 5 millisecond before another bullet can be shooted
	HE::Timer m_BulletTimer;

	std::shared_ptr<HE::Mesh> m_BulletMesh; 
public:
	PlayerControllerScript(HE::Entity* entity);

	void OnCreate() override;
	void OnUpdate(HE::Timestep& timestep) override;
	void OnDestroy() override;
	void OnCollision(HE::Entity* entity) override;

	void UpdateShooting(HE::Timestep& timestep);
	void UpdateMovement(HE::Timestep& timestep);
};
