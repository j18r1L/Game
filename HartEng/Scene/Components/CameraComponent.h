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

        void SetPrimary(bool primary);
        void SetFixedAspectRatio(bool fixedAspectRatio);
        void SetOrthographic(float size, float nearClip, float farClip);
        void SetPerspective(float fov, float aspectRatio, float nearClip, float farClip);

        bool GetPrimary() const;
        bool GetFixedAspectRatio() const;
        SceneCamera& GetCamera();
    };
}


#endif // CAMERACOMPONENT_H
