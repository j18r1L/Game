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
    const char* ComponentsTypes[] = {
        "UndefinedComponent",
        "TransformComponent",
        "MaterialComponent",
        "Texture2DComponent",
        "MeshComponent",
        "SubMeshComponent",
        "CameraComponent",
        "LightComponent",
        "ScriptComponent",
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

    Component* Entity::AddComponent(ComponentType type, Component* component)
    {
        HE_PROFILE_FUNCTION();

        if (HasComponent(type) == false)
        {
            m_Components[type] = component;
            return component;
        }
        else
        {
            HE_CORE_WARN("Failed to add component with type: '{0}'! This Component already exists in entity with name: '{1}'!", ComponentsTypes[(int)type], m_Name);
        }
        return nullptr;
    }

    Component* Entity::AddComponent(ComponentType type)
    {
        HE_PROFILE_FUNCTION();

        if (HasComponent(type) == false)
        {
            Component* component = nullptr;
            switch (type)
            {
            case (ComponentType::TransformComponent):
                component = new TransformComponent(this); break;
            case (ComponentType::MaterialComponent):
                component = new MaterialComponent(this); break;
            case (ComponentType::MeshComponent):
                component = new MeshComponent(this); break;
            case (ComponentType::SubMeshComponent):
                component = new SubMeshComponent(this); break;
            case (ComponentType::Texture2DComponent):
                component = new Texture2DComponent(this); break;
            case (ComponentType::CameraComponent):
                component = new CameraComponent(this); break;
            case (ComponentType::LightComponent):
                component = new LightComponent(this); break;
            case (ComponentType::ScriptComponent):
                component = new LightComponent(this); break;
            case (ComponentType::UndefinedComponent):
                HE_CORE_WARN("Cant create component with UndefinedType in entity with name: '{0}'", m_Name); break;
            }

            m_Components[type] = component;
            return component;
        }
        else
        {
            HE_CORE_WARN("Failed to add component with type: '{0}'! This Component already exists in entity with name: '{1}'!", ComponentsTypes[(int)type], m_Name);
        }
        return nullptr;
    }

    // Вернуть компонент из текущего геймобжекта
    Component* Entity::GetComponent(ComponentType type)
    {
        HE_PROFILE_FUNCTION();

        std::unordered_map<ComponentType, Component*>::const_iterator component = m_Components.find(type);
        if (component == m_Components.end())
        {
            HE_CORE_WARN("There is no component with type: '{0}' in entity with name: '{1}'!",  ComponentsTypes[(int)type], m_Name);
            return nullptr;
        }
        return component->second;
    }

    const std::unordered_map<ComponentType, Component*>& Entity::GetComponents()
    {
        HE_PROFILE_FUNCTION();

        return m_Components;
    }

    const std::string& Entity::GetName() const
    {
        return m_Name;
    }

    // Проверить наличие компонента type в геймобжекте
    bool Entity::HasComponent(ComponentType type)
    {
        HE_PROFILE_FUNCTION();

        return !(m_Components.find(type) == m_Components.end());
    }

    // Удалить компонент type из текущего геймобжекта
    void Entity::RemoveComponent(ComponentType type)
    {
        HE_PROFILE_FUNCTION();

        m_Components.erase(type);
    }

    void Entity::SetName(const std::string& name)
    {
        m_Name = name;
    }

}
