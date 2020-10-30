#ifndef SCENE_H
#define SCENE_H

#include "HartEng/Core/pch.h"
#include "HartEng/ECS/Entity.h"
#include "HartEng/Core/Timestep.h"
#include "HartEng/Renderer/Cameras.h"

namespace HE
{
    class Entity;

    class Scene
    {
    private:

        std::unordered_map<std::string, Entity*> m_Entities;
        std::string name;
        uint32_t objectsCount;
        friend class SceneHierarchyPanel;

    public:
        Scene(const std::string& scene_name);

        ~Scene() = default;

        Entity* CreateEntity();

        Entity* CreateEntity(const std::string& name);

        Entity* getEntity(const std::string& name);

        void DestroyEntity(const std::string& name);

        void OnUpdate(Timestep& ts);
        void OnUpdate(Timestep& ts, PerspectiveCamera& camera); // This used only in levelEditor with non-runtime camera

        void OnViewportResize(uint32_t width, uint32_t height);
    };
}

#endif // SCENE_H
