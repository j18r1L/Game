#include "PhysicsComponents.h"

void ModelPhysicsComponent::update(GLfloat deltaFrameTime)
{
    //m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(0.0f, -0.1f * deltaFrameTime, 0.0f));
}

glm::mat4* ModelPhysicsComponent::getModelMatrix()
{
    return &m_ModelMatrix;
};

void PlayerPhysicsComponent::update(GLfloat deltaFrameTime)
{
    // Сфера падает вниз
    m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(0.0f, -1.0f * deltaFrameTime, 0.0f));
}

glm::mat4* PlayerPhysicsComponent::getModelMatrix()
{
    return &m_ModelMatrix;
};


void EditorPhysicsComponent::update(GLfloat deltaFrameTime)
{
    //Находим пересечение луча с m_VAO * model_matrix

    //m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(0.0f, -0.1f * deltaFrameTime, 0.0f));
}

glm::mat4* EditorPhysicsComponent::getModelMatrix()
{
    return &m_ModelMatrix;
};
