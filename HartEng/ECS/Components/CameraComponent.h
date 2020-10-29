#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "HartEng/Renderer/Cameras.h"
#include "HartEng/ECS/Component.h"

namespace HE
{
    class CameraComponent: public Component
    {
    private:
        std::shared_ptr<Camera> m_Camera;
        bool m_Primary = false;
        bool m_FixedAspectRatio = false;
    public:
        CameraComponent(Entity* entityHandle);
        CameraComponent(Entity* entityHandle, float fov, float aspectRatio, float nearClip, float farClip, bool primary = false, bool fixedAspectRatio = false);
        CameraComponent(Entity* entityHandle, float left, float right, float bottom, float top, float nearClip, float farClip, bool primary = false, bool fixedAspectRatio = false);
        //CameraComponent(Entity* entityHandle, const glm::mat4& projection, bool primary = false, bool fixedAspectRatio = false);

        void SetPrimary(bool primary);
        void SetFixedAspectRatio(bool fixedAspectRatio);

        bool GetPrimary() const;
        bool GetFixedAspectRatio() const;
        std::shared_ptr<Camera> GetCamera();
    };
}


#endif // CAMERACOMPONENT_H
