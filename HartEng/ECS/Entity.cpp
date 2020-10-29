#include "HartEng/ECS/Entity.h"

#include "HartEng/Core/pch.h"

namespace HE
{
    Entity::Entity(Scene* sceneHandle, const std::string& name):
        m_SceneHandle(sceneHandle),
        m_Name(name)
    {
    }

    Entity::~Entity()
    {
        m_SceneHandle->DestroyEntity(m_Name);
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
    Component* Entity::GetComponent(const ComponentType& type)
    {
        std::unordered_map<ComponentType, Component*>::const_iterator component = m_Components.find(type);
        if (component == m_Components.end())
        {
            //HE_CORE_WARN("There is no component with type: {0}", type);
            //HE_CORE_ASSERT(false, "There is no component with type: "); // TODO with wich type?
            return nullptr;
        }
        return component->second;
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
        /*
        for (auto &item: m_Components)
        {
            item.second->OnUpdate();
        }
        */
    }
}
