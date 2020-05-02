#include "GameObject.h"

GameObject::GameObject(InputComponent* inputComponent, GraphicsComponent* graphicsComponent, ModelComponent* modelComponent, CameraComponent* cameraComponent, PhysicsComponent* physicsComponent):
    m_InputComponent(inputComponent),
    m_GraphicsComponent(graphicsComponent),
    m_ModelComponent(modelComponent),
    m_CameraComponent(cameraComponent),
    m_PhysicsComponent(physicsComponent)
{
}

GameObject::~GameObject()
{
    delete m_InputComponent;
    delete m_GraphicsComponent;
    delete m_ModelComponent;
    delete m_CameraComponent;
    delete m_PhysicsComponent;
}

void GameObject::update(Renderer& renderer, GLfloat deltaFrameTime)
{
    if (m_InputComponent != nullptr)
        m_InputComponent->update(deltaFrameTime);
    if (m_GraphicsComponent != nullptr)
        m_GraphicsComponent->update(renderer);
    if (m_PhysicsComponent != nullptr)
        m_PhysicsComponent->update(deltaFrameTime);
}

InputComponent* GameObject::getInputComponent()
{
    return m_InputComponent;
}



