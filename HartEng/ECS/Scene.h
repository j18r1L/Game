#ifndef SCENE_H
#define SCENE_H

#include "HartEng/Core/pch.h"
#include "HartEng/ECS/Entity.h"

namespace HE
{
    class Scene
    {
    private:

        std::unordered_map<std::string, Entity*> m_Entities;
        std::string name;
        uint32_t objectsCount;

    public:
        Scene(const std::string& scene_name):
        objectsCount(0), name(scene_name)
        {}
        ~Scene() = default;

        Entity* CreateEntity()
        {
            std::string entityName = "__obj_" + name + "_" + std::to_string(objectsCount);
            return CreateEntity(entityName);
        }

        Entity* CreateEntity(const std::string& name)
        {
            if (objectsCount == m_Entities.max_size())
            {
                HE_CORE_WARN("Max scene capacity is reached!");
                return nullptr;
            }

            // Нужно проверить стоит ли делать такую проверку (имеется ли такой геймобжект в m_Entities)
            if (m_Entities.find(name) == m_Entities.end())
            {
                Entity* entity = new Entity(name);
                m_Entities[name] = entity;
                // Добавляем компененты, которые всегда должны быть в entity, например TransformComponent
                //entity->AddComponent<TransformComponent>();
                Component* transformComponent = new TransformComponent();
                entity->AddComponent(ComponentType::TransformComponent, *transformComponent);
                objectsCount++;
                return entity;
            }
            HE_CORE_WARN("Entity with name: {0} already exists!", name);
            return nullptr;
        }

        void DestroyEntity(const std::string& name)
        {
            m_Entities.erase(name);
        }

        void OnUpdate()
        {
            for (auto& [name, entity]: m_Entities)
            {
                entity->OnUpdate();
            }
        }
    };
}

#endif // SCENE_H
