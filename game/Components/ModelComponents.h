#ifndef MODELCOMPONENTS_H
#define MODELCOMPONENTS_H

#include "Model.h"

class ModelComponent
{
public:
    virtual Model* getModel() = 0;
    virtual GLuint* getVAOxyz() = 0;

    virtual ~ModelComponent() {}
};

// TODO это не плеер компонент, а стандартный, переименовать
class PlayerModelComponent : public ModelComponent
{
private:
    Model* m_Model;
public:
    PlayerModelComponent(Model* model):
        m_Model(model) {}

    virtual Model* getModel();
    virtual GLuint* getVAOxyz() {};
    virtual ~PlayerModelComponent() {}
};


class EditorModelComponent : public ModelComponent
{
private:
    Model* m_Model;
    GLuint* m_VAOxyz;
public:
    EditorModelComponent(Model* model, GLuint* VAOxyz):
        m_Model(model),
        m_VAOxyz(VAOxyz) {}

    virtual Model* getModel();
    virtual GLuint* getVAOxyz();

    virtual ~EditorModelComponent() {}
};


#endif // MODELCOMPONENTS_H
