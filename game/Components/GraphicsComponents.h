#ifndef GRAPHICSCOMPONENTS_H
#define GRAPHICSCOMPONENTS_H

#include "ModelComponents.h"
#include "CameraComponents.h"
#include "PhysicsComponents.h"
#include "Renderer.h"

class GraphicsComponent
{
public:
    virtual ~GraphicsComponent() {}
    virtual void update(Renderer& renderer) = 0;
};



class PlayerGraphicsComponent : public GraphicsComponent
{
private:
    ModelComponent* m_ModelComponent;
    CameraComponent* m_CameraComponent;
    PhysicsComponent* m_PhysicsComponent;
public:
    PlayerGraphicsComponent(ModelComponent* modelComponent, CameraComponent* cameraComponent, PhysicsComponent* physicsComponent):
        m_ModelComponent(modelComponent),
        m_CameraComponent(cameraComponent),
        m_PhysicsComponent(physicsComponent) {}
    ~PlayerGraphicsComponent()
    {
        delete m_ModelComponent;
        delete m_CameraComponent;
    }

    virtual void update(Renderer& renderer);
};

class ModelGraphicsComponent : public GraphicsComponent
{
private:
    ModelComponent* m_ModelComponent;
    CameraComponent* m_CameraComponent;
    PhysicsComponent* m_PhysicsComponent;
public:
    ModelGraphicsComponent(ModelComponent* modelComponent, CameraComponent* cameraComponent, PhysicsComponent* physicsComponent):
        m_ModelComponent(modelComponent),
        m_CameraComponent(cameraComponent),
        m_PhysicsComponent(physicsComponent) {}
    ModelGraphicsComponent()
    {
        delete m_ModelComponent;
        delete m_CameraComponent;
    }

    virtual void update(Renderer& renderer);
};

class EditorGraphicsComponent : public GraphicsComponent
{
private:
    ModelComponent* m_ModelComponent;
    CameraComponent* m_CameraComponent;
    PhysicsComponent* m_PhysicsComponent;
public:
    EditorGraphicsComponent(ModelComponent* modelComponent, CameraComponent* cameraComponent, PhysicsComponent* physicsComponent):
        m_ModelComponent(modelComponent),
        m_CameraComponent(cameraComponent),
        m_PhysicsComponent(physicsComponent) {}
    EditorGraphicsComponent()
    {
        delete m_ModelComponent;
        delete m_CameraComponent;
    }

    virtual void update(Renderer& renderer);
};

#endif // GRAPHICSCOMPONENTS_H
