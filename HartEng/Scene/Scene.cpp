#include "HartEng/Scene/Scene.h"


#include "HartEng/Scene/Components/TransformComponent.h"
#include "HartEng/Scene/Components/MaterialComponent.h"
#include "HartEng/Scene/Components/MeshComponent.h"
#include "HartEng/Scene/Components/SubMeshComponent.h"
#include "HartEng/Scene/Components/Texture2DComponent.h"
#include "HartEng/Scene/Components/CameraComponent.h"
#include "HartEng/Scene/Components/LightComponent.h"
#include "HartEng/Scene/Components/ScriptComponent.h"
#include "HartEng/Renderer/Renderer.h"

#include "HartEng/Core/Log.h"
#include <iostream>
#include <memory>

namespace HE
{
    Scene::Scene():
        m_Entities(),
        m_Name("Undefined"),
        m_ObjectsCount(0)
    {

    }
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
            Entity* entity = new Entity(this, name, m_ObjectsCount);
            m_Entities[name] = entity;

            // Добавляем компененты, которые всегда должны быть в entity, например TransformComponent
            entity->AddComponent<TransformComponent>();
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

    Entity* Scene::GetEntity(uint32_t entityID)
    {
        HE_PROFILE_FUNCTION();

        for (auto& [name, entity] : m_Entities)
        {
            if (entity->GetID() == entityID)
            {
                return entity;
            } 
        }
        return nullptr;
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

    void Scene::OnScenePlay()
    {
        HE_PROFILE_FUNCTION();

        if (m_Play == false)
        {
            m_Play = true;
            for (auto& [name, entity] : m_Entities)
            {
                if (entity->HasComponent<ScriptComponent>())
                {
                    ScriptComponent* scriptComponent = entity->GetComponent<ScriptComponent>();
                    scriptComponent->OnCreate();
                }
            }
        }
        
    }

    void Scene::OnSceneStop()
    {
        HE_PROFILE_FUNCTION();

        if (m_Play == true)
        {
            m_Play = false;
            for (auto& [name, entity] : m_Entities)
            {
                if (entity->HasComponent<ScriptComponent>())
                {
                    ScriptComponent* scriptComponent = entity->GetComponent<ScriptComponent>();
                    scriptComponent->OnDestroy();
                }
            }
        }
    }

    // Runtime
    void Scene::OnUpdate(Timestep& ts)
    {
        HE_PROFILE_FUNCTION();

        SceneCamera* mainCamera = nullptr;
        glm::mat4 transform(1.0f);
        {
            HE_PROFILE_SCOPE("OnUpdate: update script");
            for (auto& [name, entity]: m_Entities)
            {
                if (entity->HasComponent<ScriptComponent>())
                {
                    ScriptComponent* scriptComponent = entity->GetComponent<ScriptComponent>();
                    scriptComponent->OnUpdate(ts);
                }
            }
        }
        {
            HE_PROFILE_SCOPE("OnUpdate: find mainCamera");

            // Find mainCamera
            for (auto& [name, entity]: m_Entities)
            {
                if (entity->HasComponent<CameraComponent>())
                {
                    CameraComponent* cameraComponent = entity->GetComponent<CameraComponent>();
                    if (cameraComponent->GetPrimary())
                    {
                        transform = entity->GetComponent<TransformComponent>()->GetTransform();
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
                    if (entity->HasComponent<LightComponent>())
                        lights.push_back(entity);
                }

                Renderer::BeginScene(mainCamera->GetProjection(), transform, lights);

                // For all entities
                for (auto& [name, entity]: m_Entities)
                {
                    HE_PROFILE_SCOPE(name.c_str());

                    // Render only entities with mesh
                    if (entity->HasComponent<MeshComponent>())
                    {
                        MeshComponent* meshComponent = entity->GetComponent<MeshComponent>();
                        TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
                        if (meshComponent)
                        {
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
                }
                Renderer::EndScene();
            }
        }

    }

    // Not runtime
    void Scene::OnUpdateEditor(Timestep& ts, PerspectiveCamera& camera)
    {
        HE_PROFILE_FUNCTION();
        // Get all light components
        std::vector<Entity*> lights;
        for (auto& [name, entity] : m_Entities)
        {
            if (entity->HasComponent<LightComponent>())
                lights.push_back(entity);
        }
        Renderer::BeginScene(camera, lights);
        

        // For all entities
        for (auto& [name, entity]: m_Entities)
        {
            HE_PROFILE_SCOPE(name.c_str());
            // Render only entities with mesh
            if (entity->HasComponent<MeshComponent>())
            {
                MeshComponent* meshComponent = entity->GetComponent<MeshComponent>();
                TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
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

    void Scene::OnUpdateShader(std::shared_ptr<Shader> shader, PerspectiveCamera& camera)
    {
        // For all entities
        for (auto& [name, entity] : m_Entities)
        {
            HE_PROFILE_SCOPE(name.c_str());
            // Render only entities with mesh
            if (entity->HasComponent<MeshComponent>())
            {
                MeshComponent* meshComponent = entity->GetComponent<MeshComponent>();
                TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
                auto& subMeshes = meshComponent->GetSubMeshes();
                for (auto& subMesh : subMeshes)
                {

                    shader->Bind();
                    auto& attribute = subMesh->GetAttribute();
                    Renderer::Submit(shader, attribute, entity->GetID(), transformComponent->GetTransform());

                }
            }
        }
    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        HE_PROFILE_FUNCTION();

        // Resize all our non-FixedAspectRatio cameras
        for (auto& [name, entity]: m_Entities)
        {
            if (entity->HasComponent<CameraComponent>())
            {
                CameraComponent* cameraComponent = entity->GetComponent<CameraComponent>();
                if (cameraComponent)
                {
                    if (!cameraComponent->GetFixedAspectRatio())
                    {
                        auto& camera = cameraComponent->GetCamera();
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
