#include "HartEng/Scene/Scene.h"

#include "HartEng/Scene/Components/TransformComponent.h"
#include "HartEng/Scene/Components/MaterialComponent.h"
#include "HartEng/Scene/Components/MeshComponent.h"
#include "HartEng/Scene/Components/SubMeshComponent.h"
#include "HartEng/Scene/Components/Texture2DComponent.h"
#include "HartEng/Scene/Components/CameraComponent.h"
#include "HartEng/Scene/Components/LightComponent.h"
#include "HartEng/Renderer/Renderer.h"

#include "HartEng/Core/Log.h"
#include <iostream>
#include <memory>

namespace HE
{
    Scene::Scene(const std::string& scene_name):
        m_Entities(),
        m_Name(scene_name),
        m_ObjectsCount(0)

    {
        HE_CORE_TRACE("Creating scene with name: {0}", scene_name);
    }

    Entity* Scene::CreateEntity()
    {
        std::string entityName = "__obj_" + m_Name + "_" + std::to_string(m_ObjectsCount);
        return CreateEntity(entityName);
    }

    Entity* Scene::CreateEntity(const std::string& name)
    {
        HE_PROFILE_FUNCTION();

        float size = m_Entities.max_size();
        if (m_ObjectsCount == m_Entities.max_size())
        {
            HE_CORE_WARN("Max scene capacity is reached!");
            return nullptr;
        }

        // Нужно проверить стоит ли делать такую проверку (имеется ли такой геймобжект в m_Entities)
        if (m_Entities.find(name) == m_Entities.end())
        {
            HE_CORE_TRACE("Creating entity with name: {0}", name);
            Entity* entity = new Entity(this, name);
            m_Entities[name] = entity;

            // Добавляем компененты, которые всегда должны быть в entity, например TransformComponent
            entity->AddComponent(ComponentType::TransformComponent);
            m_ObjectsCount++;

            return entity;
        }
        HE_CORE_WARN("Entity with name: {0} already exists!", name);
        return nullptr;
    }

    Entity* Scene::GetEntity(const std::string& name)
    {
        HE_PROFILE_FUNCTION();

        auto entityIterator = m_Entities.find(name);
        if (entityIterator == m_Entities.end())
        {
            HE_CORE_ASSERT(false, "There is no entity with name: {0}", name);
            return nullptr;
        }
        return entityIterator->second;
    }

    const std::unordered_map<std::string, Entity*>& Scene::GetEntities()
    {
        return m_Entities;
    }

    const std::string& Scene::GetName() const
    {
        return m_Name;
    }

    void Scene::DestroyEntity(const std::string& name)
    {
        HE_CORE_TRACE("Destroing entity with name: {0}", name);
        m_Entities.erase(name);
    }

    void Scene::Clear()
    {
        m_Entities.clear();
    }

    // Runtime
    void Scene::OnUpdate(Timestep& ts)
    {
        HE_PROFILE_FUNCTION();

        Camera* mainCamera = nullptr;
        glm::mat4 transform(1.0f);
        {
            HE_PROFILE_SCOPE("OnUpdate: find mainCamera");

            // Find mainCamera
            for (auto& [name, entity]: m_Entities)
            {
                if (entity->HasComponent(ComponentType::CameraComponent))
                {
                    CameraComponent* cameraComponent = dynamic_cast<CameraComponent*>(entity->GetComponent(ComponentType::CameraComponent));
                    if (cameraComponent->GetPrimary())
                    {
                        transform = dynamic_cast<TransformComponent*>(entity->GetComponent(ComponentType::TransformComponent))->GetTransform();
                        mainCamera = &cameraComponent->GetCamera();
                        break;
                    }
                }
                
                

            }
        }

        {
            HE_PROFILE_SCOPE("OnUpdate: Scene submit");

            if (mainCamera)
            {
                // Get all light components
                std::vector<Entity*> lights;
                for (auto& [name, entity] : m_Entities)
                {
                    if (entity->HasComponent(ComponentType::LightComponent))
                        lights.push_back(entity);
                }

                Renderer::BeginScene(mainCamera->GetProjection(), transform, lights);

                // For all entities
                for (auto& [name, entity]: m_Entities)
                {
                    HE_PROFILE_SCOPE(name.c_str());

                    // Render only entities with mesh
                    if (entity->HasComponent(ComponentType::MeshComponent))
                    {
                        MeshComponent* meshComponent = dynamic_cast<MeshComponent*>(entity->GetComponent(ComponentType::MeshComponent));
                        TransformComponent* transformComponent = dynamic_cast<TransformComponent*>(entity->GetComponent(ComponentType::TransformComponent));
                        if (meshComponent)
                        {
                            auto& subMeshes = meshComponent->GetSubMeshes();
                            for (auto& subMesh : subMeshes)
                            {
                                auto material = subMesh->GetMaterial();
                                auto shader = material->GetShader();
                                shader->Bind();
                                auto& attribute = subMesh->GetAttribute();
                                Renderer::Submit(shader, attribute, transformComponent->GetTransform(), material);

                            }
                        }
                    }
                }
                Renderer::EndScene();
            }
        }

    }

    // Not runtime
    void Scene::OnUpdate(Timestep& ts, PerspectiveCamera& camera)
    {
        HE_PROFILE_FUNCTION();
        // Get all light components
        std::vector<Entity*> lights;
        for (auto& [name, entity] : m_Entities)
        {
            if (entity->HasComponent(ComponentType::LightComponent))
                lights.push_back(entity);
        }
        Renderer::BeginScene(camera, lights);
        

        // For all entities
        for (auto& [name, entity]: m_Entities)
        {
            HE_PROFILE_SCOPE(name.c_str());
            // Render only entities with mesh
            if (entity->HasComponent(ComponentType::MeshComponent))
            {
                MeshComponent* meshComponent = dynamic_cast<MeshComponent*>(entity->GetComponent(ComponentType::MeshComponent));
                TransformComponent* transformComponent = dynamic_cast<TransformComponent*>(entity->GetComponent(ComponentType::TransformComponent));
                auto& subMeshes = meshComponent->GetSubMeshes();
                for (auto& subMesh : subMeshes)
                {
                    auto material = subMesh->GetMaterial();
                    auto shaderLibrary = material->GetShaderLibrary();
                    if (shaderLibrary != nullptr)
                    {
                        if (shaderLibrary->Exists(material->GetShaderName()))
                        {
                            auto shader = material->GetShader();
                            shader->Bind();
                            auto& attribute = subMesh->GetAttribute();
                            Renderer::Submit(shader, attribute, transformComponent->GetTransform(), material);
                        }
                    }
                }
            }
        }

        Renderer::EndScene();
    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        HE_PROFILE_FUNCTION();

        // Resize all our non-FixedAspectRatio cameras
        for (auto& [name, entity]: m_Entities)
        {
            if (entity->HasComponent(ComponentType::CameraComponent))
            {
                CameraComponent* cameraComponent = dynamic_cast<CameraComponent*>(entity->GetComponent(ComponentType::CameraComponent));
                if (cameraComponent)
                {
                    if (!cameraComponent->GetFixedAspectRatio())
                    {
                        auto camera = cameraComponent->GetCamera();
                        camera.SetViewportSize(width, height);

                    }
                }
            }
        }
    }

    void Scene::RenameEntity(std::string oldName, std::string newName)
    {
        HE_PROFILE_FUNCTION();

        Entity* entity = m_Entities.find(oldName)->second;

        m_Entities[newName] = entity;
        entity->SetName(newName);
        m_Entities.erase(oldName);
    }
}
