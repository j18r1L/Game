#include "HartEng/ECS/Scene.h"

namespace HE
{
    Scene::Scene(const std::string& scene_name):
        m_Entities(),
        name(scene_name),
        objectsCount(0)

    {

    }


    Entity* Scene::CreateEntity()
    {
        std::string entityName = "__obj_" + name + "_" + std::to_string(objectsCount);
        return CreateEntity(entityName);
    }

    Entity* Scene::CreateEntity(const std::string& name)
    {
        float size = m_Entities.max_size();
        HE_CORE_INFO("{0}", size);
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

            Component* transformComponent = new TransformComponent(entity);
            entity->AddComponent(ComponentType::TransformComponent, *transformComponent);
            objectsCount++;

            return entity;
        }
        HE_CORE_WARN("Entity with name: {0} already exists!", name);
        return nullptr;
    }

    Entity& Scene::getEntity(const std::string& name)
    {
        std::unordered_map<std::string, Entity*>::const_iterator entity = m_Entities.find(name);
        return *entity->second;
    }

    void Scene::DestroyEntity(const std::string& name)
    {
        m_Entities.erase(name);
    }

    void Scene::OnUpdate(Timestep& ts)
    {
        for (auto& [name, entity]: m_Entities)
        {
            // TODO
            // Update transform / physics first...

            // Then update graphics...

            entity->OnUpdate();
        }
    }
}
