#ifndef SCENE_H
#define SCENE_H

#include "HartEng/Core/pch.h"
#include "HartEng/ECS/Entity.h"
#include "HartEng/Core/Timestep.h"



namespace HE
{
    class Entity;

    class Scene
    {
    private:

        std::unordered_map<std::string, Entity*> m_Entities;
        std::string name;
        uint32_t objectsCount;

    public:
        Scene(const std::string& scene_name);

        ~Scene() = default;

        Entity* CreateEntity();

        Entity* CreateEntity(const std::string& name);

        Entity* getEntity(const std::string& name);

        void DestroyEntity(const std::string& name);

        void OnUpdate(Timestep& ts);
    };
}

#endif // SCENE_H
