#include "CameraComponent.h"

namespace HE
{
    CameraComponent::CameraComponent(Entity* entityHandle)
    {
        m_EntityHandle = nullptr;
        m_Type = ComponentType::CameraComponent;
    }
    /*
    CameraComponent::CameraComponent(Entity* entityHandle, const glm::mat4& projection, bool primary, bool fixedAspectRatio):
        m_Camera(projection),
        m_Primary(primary),
        m_FixedAspectRatio(fixedAspectRatio)
    {
        m_EntityHandle = entityHandle;
        m_Type = ComponentType::CameraComponent;
    }
    */
    CameraComponent::CameraComponent(Entity* entityHandle, float left, float right, float bottom, float top, float nearClip, float farClip, bool primary, bool fixedAspectRatio):
        m_Primary(primary),
        m_FixedAspectRatio(fixedAspectRatio)
    {
        m_Camera = std::shared_ptr<Camera>(new OrthographicCamera(left, right, bottom, top, nearClip, farClip));
        m_EntityHandle = entityHandle;
        m_Type = ComponentType::CameraComponent;
    }

    CameraComponent::CameraComponent(Entity* entityHandle, float fov, float aspectRatio, float nearClip, float farClip, bool primary, bool fixedAspectRatio):
        m_Primary(primary),
        m_FixedAspectRatio(fixedAspectRatio)
    {
        m_Camera = std::shared_ptr<Camera>(new PerspectiveCamera(fov, aspectRatio, nearClip, farClip));
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

    std::shared_ptr<Camera> CameraComponent::GetCamera()
    {
        return m_Camera;
    }
}

