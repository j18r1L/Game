#pragma once

#include "../HartEng/HartEng.h"


class ObstacleScript : public HE::ScriptComponent
{
private:
    float m_VerticalSpeed = 2.0f;
    float m_HorizontalSpeed = 0.1f;
    float m_RotationSpeed = 100.0f;

    bool m_RotateSide = false;

public:
    ObstacleScript(HE::Entity* entity);

    void OnCreate() override;
    void OnUpdate(HE::Timestep& timestep) override;
    void OnDestroy() override;
    void OnCollision(HE::Entity* entity) override;
};
