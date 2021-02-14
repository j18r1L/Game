#pragma once

#include "../HartEng/HartEng.h"

class BulletScript : public HE::ScriptComponent
{
private:
    float m_VerticalSpeed = 2.0f;

public:
    BulletScript(HE::Entity* entity);

    void OnCreate() override;
    void OnUpdate(HE::Timestep& timestep) override;
    void OnDestroy() override;
    void OnCollision(HE::Entity* entity) override;

    void UpdateMovement(HE::Timestep& timestep);
    void UpdateCollision(HE::Timestep& timestep);
    
};