#ifndef PHYSICSCOMPONENTS_H
#define PHYSICSCOMPONENTS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ModelComponents.h"

class PhysicsComponent
{
public:

    virtual void update(GLfloat deltaFrameTime) = 0;
    virtual glm::mat4* getModelMatrix() = 0;

    virtual ~PhysicsComponent() {}
};

class ModelPhysicsComponent: public PhysicsComponent
{
private:
    glm::mat4 m_ModelMatrix;
public:

    ModelPhysicsComponent():
        m_ModelMatrix(glm::mat4(1.0f)) {}
    virtual void update(GLfloat deltaFrameTime);
    virtual glm::mat4* getModelMatrix();

    ~ModelPhysicsComponent() {}
};

class PlayerPhysicsComponent: public PhysicsComponent
{
private:
    glm::mat4 m_ModelMatrix;
public:

    PlayerPhysicsComponent():
        m_ModelMatrix(glm::mat4(1.0f)) {}
    virtual void update(GLfloat deltaFrameTime);
    virtual glm::mat4* getModelMatrix();

    ~PlayerPhysicsComponent() {}
};


class EditorPhysicsComponent: public PhysicsComponent
{
private:
    glm::mat4 m_ModelMatrix;
    EditorModelComponent* m_EditorModelComponent;

public:

    EditorPhysicsComponent(EditorModelComponent* editorModelComponent):
        m_ModelMatrix(glm::mat4(1.0f)),
        m_EditorModelComponent(editorModelComponent) {}

    virtual void update(GLfloat deltaFrameTime);
    virtual glm::mat4* getModelMatrix();


    ~EditorPhysicsComponent()
    {
        delete m_EditorModelComponent;
    }
};
#endif // PHYSICSCOMPONENTS_H
