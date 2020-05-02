#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Renderer.h"
#include "Camera.h"
#include "Model.h"
#include "Components/GraphicsComponents.h"
#include "Components/InputComponents.h"
//#include "Components/ModelComponents.h"
//#include "Components/PhysicsComponents.h"

class GameObject
{
private:
    InputComponent* m_InputComponent;
    GraphicsComponent* m_GraphicsComponent;
    ModelComponent* m_ModelComponent;
    CameraComponent* m_CameraComponent;
    PhysicsComponent* m_PhysicsComponent;

public:
    GameObject(InputComponent* inputComponent, GraphicsComponent* graphicsComponent, ModelComponent* modelComponent, CameraComponent* cameraComponent, PhysicsComponent* physicsComponent);
    ~GameObject();
    void update(Renderer& renderer, GLfloat deltaFrameTime);
    InputComponent* getInputComponent();

};



#endif // GAMEOBJECT_H
