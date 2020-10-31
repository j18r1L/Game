#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "HartEng/Scene/SceneCamera.h"
#include "HartEng/Scene/Component.h"

namespace HE
{
    class CameraComponent: public Component
    {
    private:
        SceneCamera m_Camera;
        bool m_Primary = false;
        bool m_FixedAspectRatio = false;
    public:
        CameraComponent();
        CameraComponent(Entity* entityHandle);
        CameraComponent(Entity* entityHandle, float fov, float aspectRatio, float nearClip, float farClip, bool primary = false, bool fixedAspectRatio = false);
        CameraComponent(Entity* entityHandle, float size, float nearClip, float farClip, bool primary = false, bool fixedAspectRatio = false);

        void SetPrimary(bool primary);
        void SetFixedAspectRatio(bool fixedAspectRatio);

        bool GetPrimary() const;
        bool GetFixedAspectRatio() const;
        SceneCamera& GetCamera();
    };
}


#endif // CAMERACOMPONENT_H
