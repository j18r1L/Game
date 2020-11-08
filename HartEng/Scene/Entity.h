#ifndef ENTITY_H
#define ENTITY_H

#include "HartEng/Core/pch.h"
#include "HartEng/Scene/Scene.h"
#include "HartEng/Scene/Component.h"

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

        Component* AddComponent(ComponentType type, Component* component);
        Component* AddComponent(ComponentType type);

        // Вернуть компонент из текущего геймобжекта
        // API: GraphicsComponent = entity.GetComponent(GraphicsComponent);
        Component* GetComponent(ComponentType type);
        const std::string& GetName() const;

        // Проверить наличие компонента type в геймобжекте
        // API: entity.HasComponent(TransformComponent);
        bool HasComponent(ComponentType type);

        // Удалить компонент type из текущего геймобжекта
        // API: entity.RemoveComponent(TransformComponent);
        void RemoveComponent(ComponentType type);

        // This function can be called only in Scene class!
        void SetName(const std::string& name);
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
