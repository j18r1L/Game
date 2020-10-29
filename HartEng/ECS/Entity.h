#ifndef ENTITY_H
#define ENTITY_H

#include "HartEng/Core/pch.h"
#include "HartEng/ECS/Scene.h"
#include "HartEng/ECS/Component.h"

namespace HE
{
    class Scene;
    class Component;
    enum class ComponentType;
    class Entity
    {
    private:

        Scene* m_SceneHandle = nullptr;
        std::unordered_map<ComponentType, Component*> m_Components;
        std::string m_Name = "undefind";

    public:
        Entity() = default;
        Entity(Scene* sceneHandle, const std::string& object_name);

        virtual ~Entity();

        bool AddComponent(const ComponentType& type, Component& component);

        // Вернуть компонент из текущего геймобжекта
        // API: GraphicsComponent = entity.GetComponent(GraphicsComponent);
        Component* GetComponent(const ComponentType& type);

        // Проверить наличие компонента type в геймобжекте
        // API: entity.HasComponent(TransformComponent);
        bool HasComponent(const ComponentType& type);

        // Удалить компонент type из текущего геймобжекта
        // API: entity.RemoveComponent(TransformComponent);
        void RemoveComponent(ComponentType type);

        void OnUpdate();

        bool operator==(const Entity& other) const
        {
            return this == &other && m_SceneHandle == other.m_SceneHandle;
        }

        bool operator!=(const Entity& other) const
        {
            return !(*this == other);
        }
    };
}

#endif // ENTITY_H
