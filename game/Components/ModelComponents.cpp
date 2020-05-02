#include "ModelComponents.h"

Model* PlayerModelComponent::getModel()
{
    return m_Model;
}

Model* EditorModelComponent::getModel()
{
    return m_Model;
}

GLuint* EditorModelComponent::getVAOxyz()
{
    return m_VAOxyz;
}
