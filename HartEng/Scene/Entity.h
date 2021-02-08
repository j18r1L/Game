#pragma once

#include "HartEng/Core/pch.h"
#include "HartEng/Scene/Scene.h"
#include "HartEng/Scene/Component.h"

#include <typeindex>

namespace HE
{
    class Scene;
    class Component;
    enum class ComponentType;


    class Entity
    {
    private:
        uint32_t m_ID = 0; // Entity ID, 0 must not be valid ID!

        Scene* m_SceneHandle = nullptr;
        std::unordered_map<std::type_index, Component*> m_Components;
        std::string m_Name = "undefind";
        friend class Scene;
        // This function can be called only in Scene class!
        void SetName(const std::string& name);

    public:
        Entity() = default;
        Entity(Scene* sceneHandle, const std::string& object_name, uint32_t ID);

        virtual ~Entity();

        const std::unordered_map<std::type_index, Component*>& GetComponents();
        const std::string& GetName() const;
        uint32_t GetID() const;

        // Templated functions
        template <class T>
        void AddComponent(Component* component)
        {
            AddComponent(typeid(T), component);
        }
        void AddComponent(const std::type_index type, Component* component);
        template <class T>
        T* AddComponent()
        {
            return dynamic_cast<T*>(AddComponent(typeid(T)));
        }
        Component* AddComponent(const std::type_index type);

        template<class T>
        inline T* GetComponent()
        {
            return dynamic_cast<T*>(GetComponent(typeid(T)));
        }
        Component* GetComponent(const std::type_index type);

        template <class T>
        bool HasComponent()
        {
            return HasComponent(typeid(T));
        }
        bool HasComponent(const std::type_index type);

        template <class T>
        void RemoveComponent()
        {
            RemoveComponent(typeid(T));
        }
        void RemoveComponent(const std::type_index type);

        // Overloads
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

