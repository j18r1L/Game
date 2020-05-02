#ifndef OBJECTFABRIC_H
#define OBJECTFABRIC_H

#include "GameObject.h"

class ObjectFabric
{
private:
    CameraComponent* m_CameraComponent;
    PhysicsComponent* m_PhysicsComponent;

    std::vector<std::string> m_ModelsPath;
    std::vector<Model> m_Models;
    //TODO сделать векторы комопнентов для лучшего кеширования std::vector<PhysicsComponent> и тд

    // xyz стрелочки для editor-a
    GLuint m_xyzVAO;
public:
    ObjectFabric();

    GameObject* createPlayer(std::string path);
    GameObject* createModel(std::string path);
    GameObject* createModelEditor(std::string path);
    void createXYZ();

    GLint checkCreatedModels(std::string path) const;
    ~ObjectFabric()
    {
        delete m_CameraComponent;
        delete m_PhysicsComponent;
    }
};

#endif // OBJECTFABRIC_H
