#include "CameraComponent.h"

namespace HE
{
    CameraComponent::CameraComponent()
    {
        m_EntityHandle = nullptr;
        m_Type = ComponentType::CameraComponent;
    }

    CameraComponent::CameraComponent(Entity* entityHandle)
    {
        m_EntityHandle = entityHandle;
        m_Type = ComponentType::CameraComponent;
    }

    CameraComponent::CameraComponent(Entity* entityHandle, float size, float nearClip, float farClip, bool primary, bool fixedAspectRatio):
        m_Primary(primary),
        m_FixedAspectRatio(fixedAspectRatio)
    {
        m_Camera.SetOrthographic(size, nearClip, farClip);
        m_EntityHandle = entityHandle;
        m_Type = ComponentType::CameraComponent;
    }

    CameraComponent::CameraComponent(Entity* entityHandle, float fov, float aspectRatio, float nearClip, float farClip, bool primary, bool fixedAspectRatio):
        m_Primary(primary),
        m_FixedAspectRatio(fixedAspectRatio)
    {
        m_Camera.SetPerspective(fov, aspectRatio, nearClip, farClip);
        m_EntityHandle = entityHandle;
        m_Type = ComponentType::CameraComponent;
    }

    void CameraComponent::SetPrimary(bool primary)
    {
        m_Primary = primary;
    }

    void CameraComponent::SetFixedAspectRatio(bool fixedAspectRatio)
    {
        m_FixedAspectRatio = fixedAspectRatio;
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

