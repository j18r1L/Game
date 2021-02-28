#include "SceneHierarchyPanel.h"

#include "HartEng/Core/pch.h"
#include "HartEng/Core/Log.h"
#include "HartEng/Core/Utils.h"
#include "HartEng/Asset/AssetManager.h"

#include "glm/gtc/type_ptr.hpp"
#include <glm/gtx/matrix_decompose.hpp>

namespace HE
{
    SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<Scene>& scene, std::shared_ptr<ShaderLibrary> shaderLibrary):
        m_ShaderLibrary(shaderLibrary)
    {
        SetScene(scene);
    }

    void SceneHierarchyPanel::SetScene(const std::shared_ptr<Scene>& scene)
    {
        m_Scene = scene;
    }

    void SceneHierarchyPanel::SetShaderLibrary(std::shared_ptr<ShaderLibrary> shaderLibrary)
    {
        m_ShaderLibrary = shaderLibrary;
    }

    void SceneHierarchyPanel::SetSelectedEntity(Entity* entity)
    {
        m_SelectionContext = entity;
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Scene Hierarchy");

        m_CreateChild = false;
        for (auto& [name, entity]: m_Scene->m_Entities)
        {
            DrawEntityNode(name, entity);
        }
        // Delete entity
        if (m_DeletedEntity != "")
        {
            m_Scene->DestroyEntity(m_DeletedEntity);
            m_SelectionContext = nullptr;
            m_DeletedEntity = "";
        }
        /*
        // Create child entity
        if (m_CreateChild)
        {
            m_CreateChild = false;
            Entity* entity = m_SelectionContext->AddChild();
        }
        */

        // Deselect entity if pressed in window, but not on a entity
        //if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
        //    m_SelectionContext = nullptr;

        // Right click on blank space
        if (ImGui::BeginPopupContextWindow(0, 1, false))
        {
            if (ImGui::MenuItem("Create Empty Entity"))
                m_Scene->CreateEntity();
            ImGui::EndPopup();
        }

        ImGui::End();

        ImGui::Begin("Properties");
        if (m_SelectionContext)
        {
            // Draw Components
            DrawComponents(m_SelectionContext);

            // Add Component
            if (ImGui::Button("Add Component"))
                ImGui::OpenPopup("Add Component");

            if (ImGui::BeginPopup("Add Component"))
            {
                if (ImGui::MenuItem("Camera"))
                {
                    m_SelectionContext->AddComponent<CameraComponent>();
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::MenuItem("Mesh"))
                {
                    m_SelectionContext->AddComponent<MeshComponent>();
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::MenuItem("Light"))
                {
                    m_SelectionContext->AddComponent<LightComponent>();
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::MenuItem("Rigid Body"))
                {
                    m_SelectionContext->AddComponent<RigidBodyComponent>();
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::MenuItem("Box collider"))
                {
                    m_SelectionContext->AddComponent<BoxColliderComponent>();
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::MenuItem("Sphere collider"))
                {
                    m_SelectionContext->AddComponent<SphereColliderComponent>();
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::MenuItem("Capsule collider"))
                {
                    m_SelectionContext->AddComponent<CapsuleColliderComponent>();
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::MenuItem("Mesh Collider"))
                {
                    auto meshColliderComponent = m_SelectionContext->AddComponent<MeshColliderComponent>();
                    if (m_SelectionContext->HasComponent<MeshComponent>())
                    {
                        meshColliderComponent->SetCollisionMesh(m_SelectionContext->GetComponent<MeshComponent>()->GetMesh());
                    }
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }

        }

        ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntityNode(std::string name, Entity* entity)
    {

        ImGuiTreeNodeFlags flags = ((*m_SelectionContext == *entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx(name.c_str(), flags, name.c_str());
        if (ImGui::IsItemClicked())
        {
            m_SelectionContext = entity;
        }

        // Entity right click
        if (ImGui::BeginPopupContextItem())
        {
            m_SelectionContext = entity;
            if (ImGui::MenuItem("Delete Entity"))
                m_DeletedEntity = entity->GetName();
            if (ImGui::MenuItem("Add Entity"))
                m_CreateChild = true;
            ImGui::EndPopup();
        }

        if (opened)
        {
            ImGui::Text(name.c_str());
            ImGui::TreePop();
        }


    }

    void SceneHierarchyPanel::DrawComponents(Entity* entity)
    {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

        // Entity ID
        ImGui::Text(std::to_string(entity->GetID()).c_str());
        // Entity name
        std::string name = entity->GetName();
        static char newName[256] = "";
        for (size_t i = 0; i < name.size(); i++)
        {
            newName[i] = name[i];
        }    
        if (ImGui::InputText("Name", &newName[0], 256))
            m_Scene->RenameEntity(name, newName);

        if (entity->HasComponent<TransformComponent>())
        {
            if (ImGui::TreeNodeEx((void*)(entity->GetComponent<TransformComponent>()), treeNodeFlags, "Transform Component"))
            {
                DrawTransform(entity);
                ImGui::TreePop();
            }
        }
            
        if (entity->HasComponent<MeshComponent>())
        {
            if (ImGui::TreeNodeEx((void*)(entity->GetComponent<MeshComponent>()), treeNodeFlags, "Mesh Component"))
            {
                DrawMesh(entity);
                ImGui::TreePop();
            }
        }
            
        if (entity->HasComponent<CameraComponent>())
        {
            if (ImGui::TreeNodeEx((void*)(entity->GetComponent<CameraComponent>()), treeNodeFlags, "Camera Component"))
            {
                DrawCamera(entity);
                ImGui::TreePop();
            }
        }
            
        if (entity->HasComponent<LightComponent>())
        {
            if (ImGui::TreeNodeEx((void*)(entity->GetComponent<LightComponent>()), treeNodeFlags, "Light Component"))
            {
                DrawLight(entity);
                ImGui::TreePop();
            }
        }

        if (entity->HasComponent<RigidBodyComponent>())
        {
            if (ImGui::TreeNodeEx((void*)(entity->GetComponent<RigidBodyComponent>()), treeNodeFlags, "RigidBody Component"))
            {
                DrawRigidBody(entity);
                ImGui::TreePop();
            }
        }

        if (entity->HasComponent<BoxColliderComponent>())
        {
            if (ImGui::TreeNodeEx((void*)(entity->GetComponent<BoxColliderComponent>()), treeNodeFlags, "BoxCollider Component"))
            {
                DrawBoxCollider(entity);
                ImGui::TreePop();
            }
        }

        if (entity->HasComponent<SphereColliderComponent>())
        {
            if (ImGui::TreeNodeEx((void*)(entity->GetComponent<SphereColliderComponent>()), treeNodeFlags, "SphereCollider Component"))
            {
                DrawSphereCollider(entity);
                ImGui::TreePop();
            }
        }

        if (entity->HasComponent<CapsuleColliderComponent>())
        {
            if (ImGui::TreeNodeEx((void*)(entity->GetComponent<CapsuleColliderComponent>()), treeNodeFlags, "CapsuleCollider Component"))
            {
                DrawCapsuleCollider(entity);
                ImGui::TreePop();
            }
        }

        if (entity->HasComponent<MeshColliderComponent>())
        {
            if (ImGui::TreeNodeEx((void*)(entity->GetComponent<MeshColliderComponent>()), treeNodeFlags, "MeshCollider Component"))
            {
                DrawMeshCollider(entity);
                ImGui::TreePop();
            }
        }
            
    }

    void SceneHierarchyPanel::DrawTransform(Entity* entity)
    {
        TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();

        glm::vec3 position = transformComponent->GetPosition();
        if (ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f))
        {
            transformComponent->SetPosition(position);
        }

        // TODO fix gizmo & UI
        glm::vec3 angles = glm::degrees(transformComponent->GetRotation());
        std::swap(angles.y, angles.z);
        
        if (ImGui::DragFloat3("Rotation", glm::value_ptr(angles), 0.1f))
        {
            std::swap(angles.y, angles.z);
            transformComponent->SetRotation( angles);
        }

        glm::vec3 scale = transformComponent->GetScale();
        if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f))
        {
            transformComponent->SetScale(scale);
        }
    }
    void SceneHierarchyPanel::DrawCamera(Entity* entity)
    {
        // Remove Component
        ImGui::SameLine();
        if (ImGui::Button("+"))
            ImGui::OpenPopup("Component Settings");

        bool removeComponent = false;
        if (ImGui::BeginPopup("Component Settings"))
        {
            if (ImGui::MenuItem("Remove Component"))
            {
                removeComponent = true;
            }
            ImGui::EndPopup();
        }

        CameraComponent* cameraComponent = entity->GetComponent<CameraComponent>();
        auto& camera = cameraComponent->GetCamera();

        bool mainCamera = cameraComponent->GetPrimary();
        if (ImGui::Checkbox("Main Camera (button in development)", &mainCamera)) // TODO there can be only one main Camera
            cameraComponent->SetPrimary(mainCamera);

        bool fixedAspectRatio = cameraComponent->GetFixedAspectRatio();
        if (ImGui::Checkbox("Fixed aspect ratio", &fixedAspectRatio))
            cameraComponent->SetFixedAspectRatio(fixedAspectRatio);

        const char* projectionType[] = {"Perspective", "Orthographic"};
        const char* currentProjectionTypeString = projectionType[(int)camera.GetProjectionType()];
        if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
        {
            for (int i = 0; i < 2; i++)
            {
                bool isSelected = currentProjectionTypeString == projectionType[i];
                if (ImGui::Selectable(projectionType[i], isSelected))
                {
                    currentProjectionTypeString = projectionType[i];
                    camera.SetProjectionType((SceneCamera::ProjectionType)i);
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();

            }
            ImGui::EndCombo();
        }

        if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
        {
            float orthographicSize = camera.GetOrthographicSize();
            float orthographicNear = camera.GetOrthographicNearClip();
            float orthographicFar = camera.GetOrthographicFarClip();
            if (ImGui::DragFloat("Size", &orthographicSize))
                camera.SetOrthographicSize(orthographicSize);
            if (ImGui::DragFloat("Near", &orthographicNear))
                camera.SetOrthographicSize(orthographicNear);
            if (ImGui::DragFloat("Far", &orthographicFar))
                camera.SetOrthographicSize(orthographicFar);
        }

        if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
        {
            float perspectiveFov = camera.GetPerspectiveFov();
            float perspectiveNear = camera.GetPerspectiveNearClip();
            float perspectiveFar = camera.GetPerspectiveFarClip();
            if (ImGui::DragFloat("Fov", &perspectiveFov))
                camera.SetPerspectiveFov(perspectiveFov);
            if (ImGui::DragFloat("Near", &perspectiveNear))
                camera.SetPerspectiveNearClip(perspectiveNear);
            if (ImGui::DragFloat("Far", &perspectiveFar))
                camera.SetPerspectiveFarClip(perspectiveFar);

        }

        // Remove Component
        if (removeComponent)
            entity->RemoveComponent<CameraComponent>();
    }
    
    
    void SceneHierarchyPanel::DrawMesh(Entity* entity)
    {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

        // Remove Component
        ImGui::SameLine();
        if (ImGui::Button("+"))
            ImGui::OpenPopup("Component Settings");

        bool removeComponent = false;
        static bool createMesh = false;
        if (ImGui::BeginPopup("Component Settings"))
        {
            if (ImGui::MenuItem("Remove Component"))
            {
                removeComponent = true;
            }
            if (ImGui::MenuItem("Load new mesh"))
            {
                createMesh = true;
            }
            ImGui::EndPopup();
        }
        if (createMesh)
        {
            // TODO this should be mahaged by assetManager
#ifdef HE_PLATFORM_WINDOWS
            createMesh = false;
            std::string filepath = FileDialog::OpenFile("Mesh (*.obj)\0*.obj\0(*.fbx)\0*.fbx\0 ");
            if (!filepath.empty())
            {
                Utils::ReplaceSlash(filepath);
                const UUID& uuid = AssetManager::GetAssetIDForFile(filepath);
                if (!uuid.IsNil())
                {
                    entity->GetComponent<MeshComponent>()->SetMesh(AssetManager::GetAsset<Mesh>(uuid));
                }
            }
#elif HE_PLATFORM_LINUX
            ImGui::Begin("New mesh");
            static std::string meshName(256, '\0');
            std::string path_to_project = CMAKE_PATH;
            ImGui::Text(path_to_project.c_str());
            ImGui::SameLine();
            ImGui::InputText("", &meshName[0], 256);


            if (ImGui::Button("Accept"))
            {
                createMesh = false;
                Utils::ReplaceSlash(filepath);
                const UUID& uuid = AssetManager::GetAssetIDForFile(project_path + filepath);
                if (!uuid.IsNil())
                {
                    entity->GetComponent<MeshComponent>()->SetMesh(AssetManager::GetAsset<Mesh>(uuid));
                }

            }
            ImGui::End();
#endif
        }
        if (entity->HasComponent<MeshComponent>())
        {
            // Remove Component
            if (removeComponent)
                entity->RemoveComponent<MeshComponent>();
        }

    }
    

    void SceneHierarchyPanel::DrawLight(Entity* entity)
    {
        // Remove Component
        ImGui::SameLine();
        if (ImGui::Button("+"))
            ImGui::OpenPopup("Component Settings");

        bool removeComponent = false;
        if (ImGui::BeginPopup("Component Settings"))
        {
            if (ImGui::MenuItem("Remove Component"))
            {
                removeComponent = true;
            }
            ImGui::EndPopup();
        }

        LightComponent* lightComponent = entity->GetComponent<LightComponent>();

        const char* lightTypeChar[] = { "Directional", "Point", "Spot" };
        const char* currentLightTypeString = lightTypeChar[(int)lightComponent->GetLightType()];
        if (ImGui::BeginCombo("Light Type", currentLightTypeString))
        {
            for (int i = 0; i < 3; i++)
            {
                bool isSelected = currentLightTypeString == lightTypeChar[i];
                if (ImGui::Selectable(lightTypeChar[i], isSelected))
                {
                    currentLightTypeString = lightTypeChar[i];
                    lightComponent->SetLightType((LightType)i);
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();

            }
            ImGui::EndCombo();
        }

        bool castShadow = lightComponent->GetCastShadow();
        if (ImGui::Checkbox("Cast shadow", &castShadow))
            lightComponent->SetCastShadow(castShadow);

        if (lightComponent->GetLightType() == LightType::Directional)
        {
            glm::vec3 direction = lightComponent->GetDirection();
            glm::vec3 color = lightComponent->GetColor();
            float intensity = lightComponent->GetIntensity();
            float range = lightComponent->GetRange();

            if (ImGui::DragFloat3("Direction", glm::value_ptr(direction), 0.1f))
                lightComponent->SetDirection(direction);
            if (ImGui::DragFloat3("Color", glm::value_ptr(color), 0.01f))
                lightComponent->SetColor(color);
            if (ImGui::DragFloat("Intensity", &intensity, 0.01f))
                lightComponent->SetIntensity(intensity);
            if (ImGui::DragFloat("Range", &range))
                lightComponent->SetRange(range);
        }

        if (lightComponent->GetLightType() == LightType::Point)
        {
            glm::vec3 color = lightComponent->GetColor();
            float intensity = lightComponent->GetIntensity();
            float range = lightComponent->GetRange();

            if (ImGui::DragFloat3("Color", glm::value_ptr(color), 0.01f))
                lightComponent->SetColor(color);
            if (ImGui::DragFloat("Intensity", &intensity, 0.01f))
                lightComponent->SetIntensity(intensity);
            if (ImGui::DragFloat("Range", &range))
                lightComponent->SetRange(range);
        }

        if (lightComponent->GetLightType() == LightType::Spot)
        {
            glm::vec3 direction = lightComponent->GetDirection();
            glm::vec3 color = lightComponent->GetColor();
            float intensity = lightComponent->GetIntensity();
            float range = lightComponent->GetRange();
            float innerConeAngle = glm::degrees(glm::acos(lightComponent->GetInnerConeAngle()));
            float outerConeAngle = glm::degrees(glm::acos(lightComponent->GetOuterConeAngle()));

            if (ImGui::DragFloat3("Direction", glm::value_ptr(direction), 0.1f))
                lightComponent->SetDirection(direction);
            if (ImGui::DragFloat3("Color", glm::value_ptr(color), 0.01f))
                lightComponent->SetColor(color);
            if (ImGui::DragFloat("Intensity", &intensity, 0.01f))
                lightComponent->SetIntensity(intensity);
            if (ImGui::DragFloat("Range", &range))
                lightComponent->SetRange(range);
            if (ImGui::DragFloat("Inner Cone Angle", &innerConeAngle, 0.1f))
                lightComponent->SetInnerConeAngle(innerConeAngle);
            if (ImGui::DragFloat("outer Cone Angle", &outerConeAngle, 0.1f))
                lightComponent->SetOuterConeAngle(outerConeAngle);
        }

        // Remove Component
        if (removeComponent)
            entity->RemoveComponent<CameraComponent>();
    }

    void SceneHierarchyPanel::DrawRigidBody(Entity* entity)
    {
        // Remove Component
        ImGui::SameLine();
        if (ImGui::Button("+"))
            ImGui::OpenPopup("Component Settings");

        bool removeComponent = false;
        if (ImGui::BeginPopup("Component Settings"))
        {
            if (ImGui::MenuItem("Remove Component"))
            {
                removeComponent = true;
            }
            ImGui::EndPopup();
        }

        auto rigidBody = entity->GetComponent<RigidBodyComponent>();


        const char* bodyTypeChar[] = { "Static", "Dynamic"};
        const char* currentBodyTypeString = bodyTypeChar[(int)rigidBody->GetBodyType()];
        if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
        {
            for (int i = 0; i < 2; i++)
            {
                bool isSelected = currentBodyTypeString == bodyTypeChar[i];
                if (ImGui::Selectable(bodyTypeChar[i], isSelected))
                {
                    currentBodyTypeString = bodyTypeChar[i];
                    rigidBody->SetBodyType((RigidBodyComponent::Type)i);
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }


        float mass = rigidBody->GetMass();
        float linearDrag = rigidBody->GetLinearDrag();
        float angularDrag = rigidBody->GetAngularDrag();
        bool disableGravity = rigidBody->GetDisableGravity();
        bool isKinematic = rigidBody->GetIsKinematic();
        int layer = rigidBody->GetLayer();
        bool lockPosX = rigidBody->GetLockPositionX();
        bool lockPosY = rigidBody->GetLockPositionY();
        bool lockPosZ = rigidBody->GetLockPositionZ();
        bool lockRotX = rigidBody->GetLockRotationX();
        bool lockRotY = rigidBody->GetLockRotationY();
        bool lockRotZ = rigidBody->GetLockRotationZ();

        if (ImGui::InputInt("Layer", &layer))
        {
            if (layer >= 0)
                rigidBody->SetLayer(layer);
        }

        if (rigidBody->GetBodyType() == RigidBodyComponent::Type::Dynamic)
        {
            if (ImGui::DragFloat("Mass", &mass, 0.1f))
                rigidBody->SetMass(mass);
            if (ImGui::DragFloat("Linear Drag", &linearDrag, 0.01f))
                rigidBody->SetLinearDrag(linearDrag);
            if (ImGui::DragFloat("Angular Drag", &angularDrag, 0.01f))
                rigidBody->SetAngularDrag(angularDrag);
            if (ImGui::Checkbox("Disable gravity", &disableGravity))
                rigidBody->SetDisableGravity(disableGravity);
            if (ImGui::Checkbox("Is kinematic", &isKinematic))
                rigidBody->SetIsKinematic(isKinematic);


            if (ImGui::Checkbox("Lock position X", &lockPosX))
                rigidBody->SetLockPositionX(lockPosX);
            if (ImGui::Checkbox("Lock position Y", &lockPosY))
                rigidBody->SetLockPositionY(lockPosY);
            if (ImGui::Checkbox("Lock position Z", &lockPosZ))
                rigidBody->SetLockPositionZ(lockPosZ);

            if (ImGui::Checkbox("Lock rotation X", &lockRotX))
                rigidBody->SetLockRotationX(lockPosX);
            if (ImGui::Checkbox("Lock rotation Y", &lockRotY))
                rigidBody->SetLockRotationY(lockRotY);
            if (ImGui::Checkbox("Lock rotation Z", &lockRotZ))
                rigidBody->SetLockRotationZ(lockRotZ);
        }


        // Remove Component
        if (removeComponent)
            entity->RemoveComponent<RigidBodyComponent>();
    }

    void SceneHierarchyPanel::DrawBoxCollider(Entity* entity)
    {
        // Remove Component
        ImGui::SameLine();
        if (ImGui::Button("+"))
            ImGui::OpenPopup("Component Settings");

        bool removeComponent = false;
        bool loadPhysicsMaterial = false;
        if (ImGui::BeginPopup("Component Settings"))
        {
            if (ImGui::MenuItem("Remove Component"))
            {
                removeComponent = true;
            }
            if (ImGui::MenuItem("Load physics material"))
            {
                loadPhysicsMaterial = true;
            }
            ImGui::EndPopup();
        }

        auto boxCollider = entity->GetComponent<BoxColliderComponent>();

        if (loadPhysicsMaterial)
        {
#ifdef HE_PLATFORM_WINDOWS
            loadPhysicsMaterial = false;
            std::string filepath = FileDialog::OpenFile("Physics material (*.pm)\0");
            if (!filepath.empty())
            {
                Utils::ReplaceSlash(filepath);
                const UUID& uuid = AssetManager::GetAssetIDForFile(filepath);
                if (!uuid.IsNil())
                {
                    boxCollider->SetPhysicsMaterial(AssetManager::GetAsset<PhysicsMaterial>(uuid));
                }
            }
#elif HE_PLATFORM_LINUX
            ImGui::Begin("Load physics material");
            static std::string filepath(256, '\0');
            std::string path_to_project = CMAKE_PATH;
            ImGui::Text(path_to_project.c_str());
            ImGui::SameLine();
            ImGui::InputText("", &filepath[0], 256);


            if (ImGui::Button("Accept"))
            {
                loadPhysicsMaterial = false;
                Utils::ReplaceSlash(filepath);
                const UUID& uuid = AssetManager::GetAssetIDForFile(project_path + filepath);
                if (!uuid.IsNil())
                {
                    boxCollider->SetPhysicsMaterial(AssetManager::GetAsset<PhysicsMaterial>(uuid));
                }

                }
            ImGui::End();
#endif
        }
        

        glm::vec3 size = boxCollider->GetSize();
        glm::vec3 offset = boxCollider->GetOffset();
        bool isTrigger = boxCollider->GetTrigger();

        

        if (ImGui::DragFloat3("Size", glm::value_ptr(size), 0.1f))
            boxCollider->SetSize(size);
        if (ImGui::DragFloat3("Offset", glm::value_ptr(offset), 0.1f))
            boxCollider->SetOffset(offset);
        if (ImGui::Checkbox("Is trigger", &isTrigger))
            boxCollider->SetTrigger(isTrigger);


        auto physicsMaterial = boxCollider->GetPhysicsMaterial().get();
        if (physicsMaterial)
        {
            ImGui::Text("Physics material: ");
            ImGui::Text("Bounciness %f", physicsMaterial->Bounciness);
            ImGui::Text("Dynamic friction %f", physicsMaterial->DynamicFriction);
            ImGui::Text("Static friction %f", physicsMaterial->StaticFriction);
        }
        
        auto debugMesh = boxCollider->GetMesh().get();
        if (debugMesh)
        {
            ImGui::Text("Debug mesh: ");
            std::string filename = "Filename: " + debugMesh->FileName;
            ImGui::Text(filename.c_str());
        }

        // Remove Component
        if (removeComponent)
            entity->RemoveComponent<BoxColliderComponent>();
    }

    void SceneHierarchyPanel::DrawSphereCollider(Entity* entity)
    {
        // Remove Component
        ImGui::SameLine();
        if (ImGui::Button("+"))
            ImGui::OpenPopup("Component Settings");

        bool removeComponent = false;
        bool loadPhysicsMaterial = false;
        if (ImGui::BeginPopup("Component Settings"))
        {
            if (ImGui::MenuItem("Remove Component"))
            {
                removeComponent = true;
            }
            if (ImGui::MenuItem("Load physics material"))
            {
                loadPhysicsMaterial = true;
            }
            ImGui::EndPopup();
        }

        auto sphereCollider = entity->GetComponent<SphereColliderComponent>();

        if (loadPhysicsMaterial)
        {
#ifdef HE_PLATFORM_WINDOWS
            loadPhysicsMaterial = false;
            std::string filepath = FileDialog::OpenFile("Physics material (*.pm)\0");
            if (!filepath.empty())
            {
                Utils::ReplaceSlash(filepath);
                const UUID& uuid = AssetManager::GetAssetIDForFile(filepath);
                if (!uuid.IsNil())
                {
                    sphereCollider->SetPhysicsMaterial(AssetManager::GetAsset<PhysicsMaterial>(uuid));
                }
            }
#elif HE_PLATFORM_LINUX
            ImGui::Begin("Load physics material");
            static std::string filepath(256, '\0');
            std::string path_to_project = CMAKE_PATH;
            ImGui::Text(path_to_project.c_str());
            ImGui::SameLine();
            ImGui::InputText("", &filepath[0], 256);


            if (ImGui::Button("Accept"))
            {
                loadPhysicsMaterial = false;
                Utils::ReplaceSlash(filepath);
                const UUID& uuid = AssetManager::GetAssetIDForFile(project_path + filepath);
                if (!uuid.IsNil())
                {
                    sphereCollider->SetPhysicsMaterial(AssetManager::GetAsset<PhysicsMaterial>(uuid));
                }

            }
            ImGui::End();
#endif
        }

        float radius = sphereCollider->GetRadius();
        bool isTrigger = sphereCollider->GetTrigger();



        if (ImGui::DragFloat("Radius", &radius, 0.1f))
            sphereCollider->SetRadius(radius);
        if (ImGui::Checkbox("Is trigger", &isTrigger))
            sphereCollider->SetTrigger(isTrigger);


        auto physicsMaterial = sphereCollider->GetPhysicsMaterial().get();
        if (physicsMaterial)
        {
            ImGui::Text("Physics material: ");
            ImGui::Text("Bounciness %f", physicsMaterial->Bounciness);
            ImGui::Text("Dynamic friction %f", physicsMaterial->DynamicFriction);
            ImGui::Text("Static friction %f", physicsMaterial->StaticFriction);
        }

        auto debugMesh = sphereCollider->GetMesh().get();
        if (debugMesh)
        {
            ImGui::Text("Debug mesh: ");
            std::string filename = "Filename: " + debugMesh->FileName;
            ImGui::Text(filename.c_str());
        }

        // Remove Component
        if (removeComponent)
            entity->RemoveComponent<SphereColliderComponent>();
    }

    void SceneHierarchyPanel::DrawCapsuleCollider(Entity* entity)
    {
        // Remove Component
        ImGui::SameLine();
        if (ImGui::Button("+"))
            ImGui::OpenPopup("Component Settings");

        bool removeComponent = false;
        bool loadPhysicsMaterial = false;
        if (ImGui::BeginPopup("Component Settings"))
        {
            if (ImGui::MenuItem("Remove Component"))
            {
                removeComponent = true;
            }
            if (ImGui::MenuItem("Load physics material"))
            {
                loadPhysicsMaterial = true;
            }
            ImGui::EndPopup();
        }

        auto capsuleCollider = entity->GetComponent<CapsuleColliderComponent>();

        if (loadPhysicsMaterial)
        {
#ifdef HE_PLATFORM_WINDOWS
            loadPhysicsMaterial = false;
            std::string filepath = FileDialog::OpenFile("Physics material (*.pm)\0");
            if (!filepath.empty())
            {
                Utils::ReplaceSlash(filepath);
                const UUID& uuid = AssetManager::GetAssetIDForFile(filepath);
                if (!uuid.IsNil())
                {
                    capsuleCollider->SetPhysicsMaterial(AssetManager::GetAsset<PhysicsMaterial>(uuid));
                }
            }
#elif HE_PLATFORM_LINUX
            ImGui::Begin("Load physics material");
            static std::string filepath(256, '\0');
            std::string path_to_project = CMAKE_PATH;
            ImGui::Text(path_to_project.c_str());
            ImGui::SameLine();
            ImGui::InputText("", &filepath[0], 256);


            if (ImGui::Button("Accept"))
            {
                loadPhysicsMaterial = false;
                Utils::ReplaceSlash(filepath);
                const UUID& uuid = AssetManager::GetAssetIDForFile(project_path + filepath);
                if (!uuid.IsNil())
                {
                    capsuleCollider->SetPhysicsMaterial(AssetManager::GetAsset<PhysicsMaterial>(uuid));
                }

            }
            ImGui::End();
#endif
        }

        float radius = capsuleCollider->GetRadius();
        float height = capsuleCollider->GetHeight();
        bool isTrigger = capsuleCollider->GetTrigger();



        if (ImGui::DragFloat("Radius", &radius, 0.1f))
            capsuleCollider->SetRadius(radius);
        if (ImGui::DragFloat("Height", &height, 0.1f))
            capsuleCollider->SetRadius(height);
        if (ImGui::Checkbox("Is trigger", &isTrigger))
            capsuleCollider->SetTrigger(isTrigger);


        auto physicsMaterial = capsuleCollider->GetPhysicsMaterial().get();
        if (physicsMaterial)
        {
            ImGui::Text("Physics material: ");
            ImGui::Text("Bounciness %f", physicsMaterial->Bounciness);
            ImGui::Text("Dynamic friction %f", physicsMaterial->DynamicFriction);
            ImGui::Text("Static friction %f", physicsMaterial->StaticFriction);
        }

        auto debugMesh = capsuleCollider->GetMesh().get();
        if (debugMesh)
        {
            ImGui::Text("Debug mesh: ");
            std::string filename = "Filename: " + debugMesh->FileName;
            ImGui::Text(filename.c_str());
        }


        // Remove Component
        if (removeComponent)
            entity->RemoveComponent<CapsuleColliderComponent>();
    }

    void SceneHierarchyPanel::DrawMeshCollider(Entity* entity)
    {
        // Remove Component
        ImGui::SameLine();
        if (ImGui::Button("+"))
            ImGui::OpenPopup("Component Settings");

        bool removeComponent = false;
        bool loadPhysicsMaterial = false;
        bool loadCollisionMesh = false;
        if (ImGui::BeginPopup("Component Settings"))
        {
            if (ImGui::MenuItem("Remove Component"))
            {
                removeComponent = true;
            }
            if (ImGui::MenuItem("Load physics material"))
            {
                loadPhysicsMaterial = true;
            }
            if (ImGui::MenuItem("Load collision mesh"))
            {
                loadCollisionMesh = true;
            }
            ImGui::EndPopup();
        }

        auto meshCollider = entity->GetComponent<MeshColliderComponent>();

        if (loadPhysicsMaterial)
        {
#ifdef HE_PLATFORM_WINDOWS
            loadPhysicsMaterial = false;
            std::string filepath = FileDialog::OpenFile("Physics material (*.pm)\0");
            if (!filepath.empty())
            {
                Utils::ReplaceSlash(filepath);
                const UUID& uuid = AssetManager::GetAssetIDForFile(filepath);
                if (!uuid.IsNil())
                {
                    meshCollider->SetPhysicsMaterial(AssetManager::GetAsset<PhysicsMaterial>(uuid));
                }
            }
#elif HE_PLATFORM_LINUX
            ImGui::Begin("Load physics material");
            static std::string filepath(256, '\0');
            std::string path_to_project = CMAKE_PATH;
            ImGui::Text(path_to_project.c_str());
            ImGui::SameLine();
            ImGui::InputText("", &filepath[0], 256);


            if (ImGui::Button("Accept"))
            {
                loadPhysicsMaterial = false;
                Utils::ReplaceSlash(filepath);
                const UUID& uuid = AssetManager::GetAssetIDForFile(project_path + filepath);
                if (!uuid.IsNil())
                {
                    meshCollider->SetPhysicsMaterial(AssetManager::GetAsset<PhysicsMaterial>(uuid));
                }

            }
            ImGui::End();
#endif
        }
        else if (loadCollisionMesh)
        {
#ifdef HE_PLATFORM_WINDOWS
            loadCollisionMesh = false;
            std::string filepath = FileDialog::OpenFile("Mesh (*.obj)\0*.obj\0(*.fbx)\0*.fbx\0 ");
            if (!filepath.empty())
            {
                Utils::ReplaceSlash(filepath);
                const UUID& uuid = AssetManager::GetAssetIDForFile(filepath);
                if (!uuid.IsNil())
                {
                    meshCollider->SetCollisionMesh(AssetManager::GetAsset<Mesh>(uuid));
                }
            }
#elif HE_PLATFORM_LINUX
            ImGui::Begin("Load dubug mesh");
            static std::string meshName(256, '\0');
            std::string path_to_project = CMAKE_PATH;
            ImGui::Text(path_to_project.c_str());
            ImGui::SameLine();
            ImGui::InputText("", &meshName[0], 256);


            if (ImGui::Button("Accept"))
            {
                loadCollisionMesh = false;
                Utils::ReplaceSlash(filepath);
                const UUID& uuid = AssetManager::GetAssetIDForFile(project_path + filepath);
                if (!uuid.IsNil())
                {
                    meshCollider->SetCollisionMesh(AssetManager::GetAsset<Mesh>(uuid));
                }
            }
            ImGui::End();
#endif
        }

        bool convex = meshCollider->GetConvex();
        bool overrideMesh = meshCollider->GetOverrideMesh();
        bool isTrigger = meshCollider->GetTrigger();



        if (ImGui::Checkbox("Convex", &convex))
            meshCollider->SetConvex(convex);
        if (ImGui::Checkbox("Override mesh", &overrideMesh))
            meshCollider->SetOverrideMesh(overrideMesh);
        if (ImGui::Checkbox("Is trigger", &isTrigger))
            meshCollider->SetTrigger(isTrigger);


        auto physicsMaterial = meshCollider->GetPhysicsMaterial().get();
        if (physicsMaterial)
        {
            ImGui::Text("Physics material: ");
            ImGui::Text("Bounciness: %f", physicsMaterial->Bounciness);
            ImGui::Text("Dynamic friction: %f", physicsMaterial->DynamicFriction);
            ImGui::Text("Static friction: %f", physicsMaterial->StaticFriction);
        }

        auto collisionMesh = meshCollider->GetCollisionMesh().get();
        if (collisionMesh)
        {
            ImGui::Text("Collision mesh: ");
            std::string filename = "Filename: " + collisionMesh->FileName;
            ImGui::Text(filename.c_str());
        }


        // Remove Component
        if (removeComponent)
            entity->RemoveComponent<MeshColliderComponent>();
    }

    Entity* SceneHierarchyPanel::GetSelectedEntity() const
    {
        return m_SelectionContext;
    }
}

