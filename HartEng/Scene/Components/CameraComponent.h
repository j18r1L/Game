#pragma once

#include "HartEng/Scene/SceneCamera.h"
#include "HartEng/Scene/Component.h"
#include "HartEng/Renderer/Mesh.h"

namespace HE
{
    class CameraComponent: public Component
    {
    private:
        SceneCamera m_Camera;
        bool m_Primary = false;
        bool m_FixedAspectRatio = false;

        std::shared_ptr<Mesh> m_DebugMesh;

        void LoadCameraDebugMesh();
    public:
        CameraComponent();
        CameraComponent(Entity* entityHandle);

        virtual ~CameraComponent()
        {

        }

        void SetPrimary(bool primary);
        void SetFixedAspectRatio(bool fixedAspectRatio);
        void SetOrthographic(float size, float nearClip, float farClip);
        void SetPerspective(float fov, float aspectRatio, float nearClip, float farClip);

        bool GetPrimary() const;
        bool GetFixedAspectRatio() const;
        SceneCamera& GetCamera();
        std::shared_ptr<Mesh> GetMesh() { return m_DebugMesh; }
    };
}

