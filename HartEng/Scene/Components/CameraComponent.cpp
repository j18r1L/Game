#include "CameraComponent.h"

#include "HartEng/Asset/AssetManager.h"

namespace HE
{
    CameraComponent::CameraComponent()
    {
        m_EntityHandle = nullptr;
        LoadCameraDebugMesh();
    }

    CameraComponent::CameraComponent(Entity* entityHandle)
    {
        m_EntityHandle = entityHandle;
        LoadCameraDebugMesh();
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

    void CameraComponent::LoadCameraDebugMesh()
    {
        std::string pathToProject = CMAKE_PATH;
        const UUID uuid = AssetManager::GetAssetIDForFile(pathToProject + "/assets/meshes/obj/camera/camera.obj");
        if (!uuid.IsNil())
        {
            m_DebugMesh = AssetManager::GetAsset<Mesh>(uuid);
        }
        else
        {
            HE_CORE_ERROR("CameraComponent::LoadCameraDebugMesh AssetID for camera debug mesh is not valid!");
        }
    }
}

