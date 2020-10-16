#ifndef ENTITY_H
#define ENTITY_H

#include "HartEng/Core/pch.h"
#include "HartEng/ECS/Scene.h"
#include "HartEng/ECS/Components.h"

namespace HE
{
    class Entity
    {
    private:

        //Scene* m_SceneHandle;
        std::unordered_map<ComponentType, Component*> m_Components;
        std::string name;

    public:
        Entity(const std::string& object_name):
            name(object_name)
        {
            // m_SceneHandle = SceneManager()->getActiveScene();
        }

        ~Entity()
        {
            //m_SceneHandle->DestroyEntity(name);
        }

        bool AddComponent(const ComponentType& type, Component& component)
        {
            if (HasComponent(type) == false)
            {
                m_Components[type] = &component;
                return true;
            }
            return false;
        }

        // Вернуть компонент из текущего геймобжекта
        // API: GraphicsComponent = entity.GetComponent(GraphicsComponent);
        Component& GetComponent(const ComponentType& type)
        {
            std::unordered_map<ComponentType, Component*>::const_iterator got = m_Components.find(type);
            return *got->second;
        }

        // Проверить наличие компонента type в геймобжекте
        // API: entity.HasComponent(TransformComponent);
        bool HasComponent(const ComponentType& type)
        {
            return !(m_Components.find(type) == m_Components.end());
        }

        // Удалить компонент type из текущего геймобжекта
        // API: entity.RemoveComponent(TransformComponent);
        void RemoveComponent(ComponentType type)
        {
            m_Components.erase(type);
        }

        void OnUpdate()
        {
            for (auto &item: m_Components)
            {
                item.second->OnUpdate();
            }
        }
    };
}

#endif // ENTITY_H
