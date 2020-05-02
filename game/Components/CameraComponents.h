#ifndef CAMERACOMPONENTS_H
#define CAMERACOMPONENTS_H

#include "Camera.h"

class CameraComponent
{
public:
    virtual ~CameraComponent() {}
    virtual Camera* getCamera() = 0;
};


class PlayerCameraComponent : public CameraComponent
{
private:
    Camera m_Camera;
public:
    PlayerCameraComponent():
        m_Camera(Camera(glm::vec3(0.0f, 0.0f, 0.0f))) {}

    virtual ~PlayerCameraComponent() {}
    virtual Camera* getCamera();
};

#endif // CAMERACOMPONENTS_H
