#include "HartEng/ECS/Entity.h"

namespace HE
{
    Entity::Entity(const std::string& object_name):
        name(object_name)
    {
        // m_SceneHandle = SceneManager()->getActiveScene();
    }

    Entity::~Entity()
    {
        //m_SceneHandle->DestroyEntity(name);
    }

    bool Entity::AddComponent(const ComponentType& type, Component& component)
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
    Component& Entity::GetComponent(const ComponentType& type)
    {
        std::unordered_map<ComponentType, Component*>::const_iterator component = m_Components.find(type);
        return *component->second;
    }

    // Проверить наличие компонента type в геймобжекте
    // API: entity.HasComponent(TransformComponent);
    bool Entity::HasComponent(const ComponentType& type)
    {
        return !(m_Components.find(type) == m_Components.end());
    }

    // Удалить компонент type из текущего геймобжекта
    // API: entity.RemoveComponent(TransformComponent);
    void Entity::RemoveComponent(ComponentType type)
    {
        m_Components.erase(type);
    }

    void Entity::OnUpdate()
    {
        for (auto &item: m_Components)
        {
            item.second->OnUpdate();
        }
    }
}
