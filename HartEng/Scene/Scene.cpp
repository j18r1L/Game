#include "HartEng/Scene/Scene.h"


#include "HartEng/Scene/Components/TransformComponent.h"
#include "HartEng/Scene/Components/Texture2DComponent.h"
#include "HartEng/Scene/Components/CollidersComponent.h"
#include "HartEng/Scene/Components/CameraComponent.h"
#include "HartEng/Scene/Components/ScriptComponent.h"
#include "HartEng/Scene/Components/LightComponent.h"
#include "HartEng/Scene/Components/MeshComponent.h"
#include "HartEng/Scene/Components/TagComponent.h"


#include "HartEng/Renderer/SceneRenderer.h"
#include "HartEng/Renderer/Renderer.h"

#include "HartEng/Physics/Physics.h"

#include "HartEng/Core/Application.h"
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
        Physics::CreateScene();
    }
    Scene::Scene(const std::string& scene_name):
        m_Entities(),
        m_Name(scene_name),
        m_ObjectsCount(0)

    {
        HE_CORE_TRACE("Creating scene with name: {0}", scene_name);
        Physics::CreateScene();
    }

    Scene::Scene(const Scene& other) :
        m_Entities(other.GetEntities()),
        m_Name(other.GetName()),
        m_ObjectsCount(m_Entities.size())
    {
        //HE_CORE_TRACE("Copy scene {0} to new scene with name: {1}", other.GetName(), m_Name);
        Physics::CreateScene();
    }

    Entity* Scene::CreateEntity()
    {
        std::string entityName = "__obj_" + m_Name + "_" + std::to_string(m_ObjectsCount);
        return CreateEntity(entityName);
    }

    Entity* Scene::CreateEntity(const Entity& other)
    {
        std::string entityName = "__obj_" + m_Name + "_" + std::to_string(m_ObjectsCount);

        Entity* entity = new Entity(this, entityName, m_ObjectsCount + 1); // 0 is not valid ID
        m_Entities[entityName] = entity;
        m_ObjectsCount++;

        for (auto [type, component] : other.GetComponents())
        {
            auto component_new = entity->AddComponent(type);
            component_new->Copy(component);
        }
        return entity;
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
            Entity* entity = new Entity(this, name, m_ObjectsCount+1); // 0 is not valid ID
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
            HE_CORE_ASSERT(false, "There is no entity with name: " + name);
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

    Entity Scene::GetMainCameraEntity()
    {
        // Find mainCamera
        for (auto& [name, entity] : m_Entities)
        {
            if (entity->HasComponent<CameraComponent>())
            {
                CameraComponent* cameraComponent = entity->GetComponent<CameraComponent>();
                if (cameraComponent->GetPrimary())
                {
                    return *entity;
                }
            }
        }
        return {};
    }

    const std::unordered_map<std::string, Entity*>& Scene::GetEntities()
    {
        return m_Entities;
    }

    const std::unordered_map<std::string, Entity*>& Scene::GetEntities() const
    {
        return m_Entities;
    }

    const std::string& Scene::GetName() const
    {
        return m_Name;
    }

    const LightStruct& Scene::GetLights() const
    {
        return m_Lights;
    }

    Entity* Scene::FindEntityByTag(const std::string& tag)
    {
        HE_PROFILE_FUNCTION();

        for (auto& [name, entity] : m_Entities)
        {
            if (entity->HasComponent<TagComponent>())
            {
                if (entity->GetComponent<TagComponent>()->GetTag() == tag)
                {
                    return entity;
                }
            }
        }
        return nullptr;
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

    void Scene::OnRuntimeStart()
    {
        HE_PROFILE_FUNCTION();

        if (m_Play == false)
        {
            m_Play = true;
            for (auto& [name, entity] : m_Entities)
            {
                if (entity->HasComponent<RigidBodyComponent>())
                {
                    Physics::CreateActor(*entity);
                }

            }


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

    void Scene::OnRuntimeStop()
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
            Physics::DestroyScene();
        }
        
    }

    // Update scripts
    void Scene::OnUpdate(Timestep& ts)
    {
        HE_PROFILE_FUNCTION();
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
            HE_PROFILE_SCOPE("OnUpdate: update physics");
            Physics::Simulate(ts);
        }
    }

    // Runtime render
    void Scene::OnRenderRuntime(Timestep& ts)
    {
        // Find main camera
        Entity cameraEntity = GetMainCameraEntity();
        if (!cameraEntity.GetID())
        {
            HE_CORE_ASSERT(cameraEntity.GetID(), "Scene does not contain any cameras!");
            return;
        }

        // Add all lights to LightEnvironment and copy them to SceneRenderer in BeginScene
        ProcessLights();

        // Process camera entity
        glm::mat4 cameraViewMatrix = glm::inverse(cameraEntity.GetComponent<TransformComponent>()->GetTransform());
        SceneCamera& mainCamera = cameraEntity.GetComponent<CameraComponent>()->GetCamera();

        // Prepare CameraMatrixSctuct and keep track of previous frame matrices 
        m_CameraMatrixStruct.projPrevFrame = m_CameraMatrixStruct.proj;
        m_CameraMatrixStruct.proj = mainCamera.GetProjection();
        m_CameraMatrixStruct.projInv = glm::inverse(m_CameraMatrixStruct.proj);
        
        m_CameraMatrixStruct.viewPrevFrame = m_CameraMatrixStruct.view;
        m_CameraMatrixStruct.view = cameraViewMatrix;
        m_CameraMatrixStruct.viewInv = glm::inverse(m_CameraMatrixStruct.view);

        m_CameraMatrixStruct.projViewPrevFrame = m_CameraMatrixStruct.projView;
        m_CameraMatrixStruct.projView = m_CameraMatrixStruct.proj * m_CameraMatrixStruct.view;
        m_CameraMatrixStruct.projViewInv = glm::inverse(m_CameraMatrixStruct.projView);

        float nearClip = 0.1f;
        float farClip = 1.0f;
        float fov = 45.0f;
        switch (mainCamera.GetProjectionType())
        {
        case SceneCamera::ProjectionType::Perspective:
            nearClip = mainCamera.GetPerspectiveNearClip();
            farClip = mainCamera.GetPerspectiveFarClip();
            fov = mainCamera.GetPerspectiveFov();
            break;
        case SceneCamera::ProjectionType::Orthographic: 
            nearClip = mainCamera.GetOrthographicNearClip();
            farClip = mainCamera.GetOrthographicFarClip();
            break;
        }

       
        {
            HE_PROFILE_SCOPE("OnUpdate: Scene submit");

            SceneRenderer::BeginScene(this, { mainCamera, m_CameraMatrixStruct, nearClip, farClip, fov }, ts);

            // For all entities
            for (auto& [name, entity] : m_Entities)
            {
                HE_PROFILE_SCOPE(name.c_str());

                // Render only entities with mesh
                if (entity->HasComponent<MeshComponent>())
                {
                    MeshComponent* meshComponent = entity->GetComponent<MeshComponent>();
                    TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();

                    meshComponent->GetMesh().get()->OnUpdate(ts);
                    SceneRenderer::SubmitMesh(meshComponent->GetMesh(), transformComponent->GetTransform(), nullptr);
                    
                }
            }
            SceneRenderer::EndScene();
        }
    }

    // Render editor
    void Scene::OnRenderEditor(Timestep& ts, PerspectiveCamera& camera)
    {
        HE_PROFILE_FUNCTION();
        {
            HE_PROFILE_SCOPE("OnUpdate: Scene submit");

            SceneCamera sceneCamera;
            sceneCamera.SetPerspective(camera.GetFov(), camera.GetAspectRatio(), camera.GetNear(), camera.GetFar());

            // Add all lights to LightEnvironment and copy them to SceneRenderer in BeginScene
            ProcessLights();

            m_CameraMatrixStruct.projPrevFrame = m_CameraMatrixStruct.proj;
            m_CameraMatrixStruct.proj = sceneCamera.GetProjection();
            m_CameraMatrixStruct.projInv = glm::inverse(m_CameraMatrixStruct.proj);

            m_CameraMatrixStruct.viewPrevFrame = m_CameraMatrixStruct.view;
            m_CameraMatrixStruct.view = camera.GetView();
            m_CameraMatrixStruct.viewInv = glm::inverse(m_CameraMatrixStruct.view);

            m_CameraMatrixStruct.projViewPrevFrame = m_CameraMatrixStruct.projView;
            m_CameraMatrixStruct.projView = m_CameraMatrixStruct.proj * m_CameraMatrixStruct.view;
            m_CameraMatrixStruct.projViewInv = glm::inverse(m_CameraMatrixStruct.projView);

            SceneRenderer::BeginScene(this, { sceneCamera, m_CameraMatrixStruct, camera.GetNear(), camera.GetFar(), camera.GetFov()}, ts);

           
            auto& material = Material::Create(Renderer::GetShaderLibrary().get()->Get("EntityID"));
            

            // For all entities
            for (auto& [name, entity] : m_Entities)
            {
                HE_PROFILE_SCOPE(name.c_str());

                // Render only entities with mesh
                if (entity->HasComponent<MeshComponent>())
                {
                    MeshComponent* meshComponent = entity->GetComponent<MeshComponent>();
                    TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
                    if (meshComponent)
                    {
                        meshComponent->GetMesh().get()->OnUpdate(ts);

                        // Submit mesh to geometry pass
                        SceneRenderer::SubmitMesh(meshComponent->GetMesh(), transformComponent->GetTransform());

                        // Submit mesh to entityID pass
                        uint32_t entityID = entity->GetID();

                        // Create Material Instance to store EntityID
                        auto& materialInstance = MaterialInstance::Create(material);
                        materialInstance.get()->Set("u_EntityID", entityID);

                        SceneRenderer::SubmitEntityIDMesh(meshComponent->GetMesh(), transformComponent->GetTransform(), materialInstance);
                    }
                }


                // Render debugmeshes
                if (entity->HasComponent<CameraComponent>())
                {
                    auto cameraComponent = entity->GetComponent<CameraComponent>();
                    auto transformComponent = entity->GetComponent<TransformComponent>();

                    SceneRenderer::SubmitColliderMesh(cameraComponent->GetMesh(), transformComponent->GetTransform());
                }

                if (entity->HasComponent<BoxColliderComponent>())
                {
                   auto boxColliderComponent = entity->GetComponent<BoxColliderComponent>();
                   auto transformComponent = entity->GetComponent<TransformComponent>();

                   SceneRenderer::SubmitColliderMesh(boxColliderComponent->GetMesh(), glm::translate(transformComponent->GetTransform(), boxColliderComponent->GetOffset()));
                }

                if (entity->HasComponent<SphereColliderComponent>())
                {
                    auto sphereColliderComponent = entity->GetComponent<SphereColliderComponent>();
                    auto transformComponent = entity->GetComponent<TransformComponent>();

                    SceneRenderer::SubmitColliderMesh(sphereColliderComponent->GetMesh(), transformComponent->GetTransform());
                }

                if (entity->HasComponent<CapsuleColliderComponent>())
                {
                    auto capsuleColliderComponent = entity->GetComponent<CapsuleColliderComponent>();
                    auto transformComponent = entity->GetComponent<TransformComponent>();

                    SceneRenderer::SubmitColliderMesh(capsuleColliderComponent->GetMesh(), transformComponent->GetTransform());
                }

                /*
                if (entity->HasComponent<MeshColliderComponent>())
                {
                    auto capsuleColliderComponent = entity->GetComponent<MeshColliderComponent>();
                    auto transformComponent = entity->GetComponent<TransformComponent>();

                    for (auto& mesh : capsuleColliderComponent->GetProcessedMeshes())
                    {
                        SceneRenderer::SubmitColliderMesh(mesh, transformComponent->GetTransform());
                    }
                }
                */
            }
            SceneRenderer::EndScene();
        }
    }

    void Scene::ProcessLights()
    {
        // Process lights
        {
            HE_PROFILE_SCOPE("Scene::OnRenderRuntime Process Lights");
            m_Lights = LightStruct();
            std::vector<LightMatrixStruct> lightMatrixStruct(HE_MAX_LIGHT_COUNT);

            uint32_t directionalLightCount = 0;

            uint32_t pointLightCount = 0;

            uint32_t spotLightCount = 0;

            for (auto& [name, entity] : m_Entities)
            {
                // Pretty dumb, but there can be error that entity is nullptr
                if (entity)
                {
                    if (entity->HasComponent<LightComponent>())
                    {
                        auto lightComponent = entity->GetComponent<LightComponent>();
                        auto transformComponent = entity->GetComponent<TransformComponent>();
                        LightType lightType = lightComponent->GetLightType();
                        if (lightType == LightType::Directional)
                        {
                            lightMatrixStruct.insert(lightMatrixStruct.begin() + directionalLightCount,
                                {
                                    // Position, range
                                    { transformComponent->GetPosition(), lightComponent->GetRange() },
                                    // Color, intensity
                                    { lightComponent->GetColor(), lightComponent->GetIntensity() },
                                    // spot inner, outer, scale, offset angles
                                    { 0.0f, 0.0f, 0.0f, 0.0f }

                                });

                            directionalLightCount++;
                        }
                        else if (lightType == LightType::Point)
                        {
                            lightMatrixStruct.insert(lightMatrixStruct.begin() + directionalLightCount + pointLightCount,
                                {
                                    // Position, range
                                    { transformComponent->GetPosition(), lightComponent->GetRange() },
                                    // Color, intensity
                                    { lightComponent->GetColor(), lightComponent->GetIntensity() },
                                    // spot inner, outer, scale, offset angles
                                    { 0.0f, 0.0f, 0.0f, 0.0f }

                                });
                            pointLightCount++;
                        }
                        else if (lightType == LightType::Spot)
                        {
                            lightMatrixStruct.insert(lightMatrixStruct.begin() + directionalLightCount + pointLightCount + spotLightCount,
                                {
                                    // Position, range
                                    { transformComponent->GetPosition(), lightComponent->GetRange() },
                                    // Color, intensity
                                    { lightComponent->GetColor(), lightComponent->GetIntensity() },
                                    // spot inner, outer, scale, offset angles
                                    { lightComponent->GetInnerConeAngle() , lightComponent->GetOuterConeAngle(), 1.0f, 0.0f }

                                });
                            spotLightCount++;
                        }
                    }
                }
            }

            // Set begin indices & counts
            m_Lights.directionalLightBeginIndex = 0;
            m_Lights.directionalLightCount = directionalLightCount;

            m_Lights.pointLightBeginIndex = m_Lights.directionalLightCount;
            m_Lights.pointLightCount = pointLightCount;

            m_Lights.spotLightBeginIndex = m_Lights.directionalLightCount + m_Lights.pointLightCount;
            m_Lights.spotLightCount = spotLightCount;
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
