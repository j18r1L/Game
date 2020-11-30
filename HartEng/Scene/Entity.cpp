#include "HartEng/Scene/Entity.h"

#include "HartEng/Core/pch.h"
#include "HartEng/Scene/Components/TransformComponent.h"
#include "HartEng/Scene/Components/MaterialComponent.h"
#include "HartEng/Scene/Components/Texture2DComponent.h"
#include "HartEng/Scene/Components/MeshComponent.h"
#include "HartEng/Scene/Components/SubMeshComponent.h"
#include "HartEng/Scene/Components/CameraComponent.h"
#include "HartEng/Scene/Components/LightComponent.h"
#include "HartEng/Scene/Components/ScriptComponent.h"

namespace HE
{
    std::unordered_map<std::type_index, std::string> ComponentsTypes =
    {
        {std::type_index(typeid(TransformComponent)), "TransformComponent"},
        {std::type_index(typeid(MaterialComponent)), "MaterialComponent"},
        {std::type_index(typeid(Texture2DComponent)), "Texture2DComponent"},
        {std::type_index(typeid(MeshComponent)), "MeshComponent"},
        {std::type_index(typeid(SubMeshComponent)), "SubMeshComponent"},
        {std::type_index(typeid(CameraComponent)), "CameraComponent"},
        {std::type_index(typeid(LightComponent)), "LightComponent"},
        {std::type_index(typeid(ScriptComponent)), "ScriptComponent"},
    };

    Entity::Entity(Scene* sceneHandle, const std::string& name):
        m_SceneHandle(sceneHandle),
        m_Name(name)
    {
    }

    Entity::~Entity()
    {
        //m_SceneHandle->DestroyEntity(m_Name);
    }

    void Entity::AddComponent(const std::type_index type, Component* component)
    {
        HE_PROFILE_FUNCTION();

        if (HasComponent(type) == false)
        {
            m_Components.insert(std::make_pair(type, component));
        }
        else
        {
            HE_CORE_WARN("Failed to add component with type: '{0}'! This Component already exists in entity with name: '{1}'!", ComponentsTypes[type], m_Name);
        }
    }

    Component* Entity::AddComponent(const std::type_index type)
    {
        HE_PROFILE_FUNCTION();

        if (HasComponent(type) == false)
        {
            Component* component = nullptr;
            if (type == std::type_index(typeid(TransformComponent)))
                component = new TransformComponent(this);
            else if (type == std::type_index(typeid(MaterialComponent)))
                component = new MaterialComponent(this);
            else if (type == std::type_index(typeid(MeshComponent)))
                component = new MeshComponent(this);
            else if (type == std::type_index(typeid(SubMeshComponent)))
                component = new SubMeshComponent(this);
            else if (type == std::type_index(typeid(Texture2DComponent)))
                component = new Texture2DComponent(this);
            else if (type == std::type_index(typeid(CameraComponent)))
                component = new CameraComponent(this);
            else if (type == std::type_index(typeid(LightComponent)))
                component = new LightComponent(this);
            else if (type == std::type_index(typeid(ScriptComponent)))
                component = new ScriptComponent(this);

            m_Components.insert(std::make_pair(type, component));
            return component;
        }
        else
        {
            HE_CORE_WARN("Failed to add component with type: '{0}'! This Component already exists in entity with name: '{1}'!", ComponentsTypes[type], m_Name);
        }
        return nullptr;
    }

    // Вернуть компонент из текущего геймобжекта
    Component* Entity::GetComponent(const std::type_index type)
    {
        HE_PROFILE_FUNCTION();

        std::unordered_map<std::type_index, Component*>::const_iterator component = m_Components.find(type);
        if (component == m_Components.end())
        {
            HE_CORE_WARN("There is no component with type: '{0}' in entity with name: '{1}'!",  ComponentsTypes[type], m_Name);
            return nullptr;
        }
        return component->second;
    }

    const std::unordered_map<std::type_index, Component*>& Entity::GetComponents()
    {
        HE_PROFILE_FUNCTION();

        return m_Components;
    }

    const std::string& Entity::GetName() const
    {
        return m_Name;
    }

    // Проверить наличие компонента type в геймобжекте
    bool Entity::HasComponent(const std::type_index type)
    {
        HE_PROFILE_FUNCTION();

        return !(m_Components.find(type) == m_Components.end());
    }

    // Удалить компонент type из текущего геймобжекта
    void Entity::RemoveComponent(const std::type_index type)
    {
        HE_PROFILE_FUNCTION();

        m_Components.erase(type);
    }

    void Entity::SetName(const std::string& name)
    {
        m_Name = name;
    }

}
