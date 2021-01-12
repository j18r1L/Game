#include "CameraComponent.h"

namespace HE
{
    CameraComponent::CameraComponent()
    {
        m_EntityHandle = nullptr;
    }

    CameraComponent::CameraComponent(Entity* entityHandle)
    {
        m_EntityHandle = entityHandle;
    }

    void CameraComponent::SetPrimary(bool primary)
    {
        m_Primary = primary;
    }

    void CameraComponent::SetFixedAspectRatio(bool fixedAspectRatio)
    {
        m_FixedAspectRatio = fixedAspectRatio;
    }

    void CameraComponent::SetOrthographic(float size, float nearClip, float farClip)
    {
        m_Camera.SetOrthographic(size, nearClip, farClip);
    }

    void CameraComponent::SetPerspective(float fov, float aspectRatio, float nearClip, float farClip)
    {
        m_Camera.SetPerspective(fov, aspectRatio, nearClip, farClip);
    }

    bool CameraComponent::GetPrimary() const
    {
        return m_Primary;
    }

    bool CameraComponent::GetFixedAspectRatio() const
    {
        return m_FixedAspectRatio;
    }

    SceneCamera& CameraComponent::GetCamera()
    {
        return m_Camera;
    }
}

