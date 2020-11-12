#include "SceneHierarchyPanel.h"

#include "HartEng/Core/pch.h"
#include "HartEng/Core/Log.h"
#include "glm/gtc/type_ptr.hpp"

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

    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Scene Hierarchy");

        for (auto& [name, entity]: m_Scene->m_Entities)
        {
            DrawEntityNode(name, entity);
        }
        if (m_DeletedEntity != "")
        {
            m_Scene->DestroyEntity(m_DeletedEntity);
            m_SelectionContext = nullptr;
            m_DeletedEntity = "";
        }

        //Deselect entity if pressed in window, but not on a entity
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectionContext = nullptr;

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
                    m_SelectionContext->AddComponent(ComponentType::CameraComponent);
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::MenuItem("Material"))
                {
                    m_SelectionContext->AddComponent(ComponentType::MaterialComponent);
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::MenuItem("Mesh"))
                {
                    m_SelectionContext->AddComponent(ComponentType::MeshComponent);
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

        // Delete entity
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete Entity"))
                m_DeletedEntity = entity->GetName();
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

        // Entity name
        auto& name = entity->GetName();

        static std::string EntityName(256, '\0');
        EntityName = name;
        if (ImGui::InputText("Name", &EntityName[0], 256))
            m_Scene->RenameEntity(name, EntityName);

        if (entity->HasComponent(ComponentType::TransformComponent))
            if (ImGui::TreeNodeEx((void*)(ComponentType::TransformComponent), treeNodeFlags, "Transform Component"))
            {
                DrawTransform(entity);
                ImGui::TreePop();
            }
        if (entity->HasComponent(ComponentType::MaterialComponent))
            if (ImGui::TreeNodeEx((void*)(ComponentType::MaterialComponent), treeNodeFlags, "Material Component"))
            {
                DrawMaterial(entity);
                ImGui::TreePop();
            }
        if (entity->HasComponent(ComponentType::MeshComponent))
            if (ImGui::TreeNodeEx((void*)(ComponentType::MeshComponent), treeNodeFlags, "Mesh Component"))
            {
                DrawMesh(entity);
                ImGui::TreePop();
            }
        if (entity->HasComponent(ComponentType::CameraComponent))
            if (ImGui::TreeNodeEx((void*)(ComponentType::CameraComponent), treeNodeFlags, "Camera Component"))
            {
                DrawCamera(entity);
                ImGui::TreePop();
            }
    }

    void SceneHierarchyPanel::DrawTransform(Entity* entity)
    {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

        TransformComponent* transform = dynamic_cast<TransformComponent*>(entity->GetComponent(ComponentType::TransformComponent));

        glm::vec3 position = transform->GetTranslation();
        if (ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f))
        {
            transform->SetTranslation(position);
        }



        glm::quat rotation = transform->GetRotation();
        glm::vec3 angles = glm::eulerAngles(rotation);
        /*
        if (ImGui::DragFloat3("Rotation", glm::value_ptr(angles), 0.1f))
        {
            transform->SetRotation(angles);
        }
        */
        if (ImGui::InputFloat("", &angles.x))
            transform->SetScale(angles);
        ImGui::SameLine();
        if (ImGui::InputFloat("", &angles.y))
                transform->SetScale(angles);
        ImGui::SameLine();
        if (ImGui::InputFloat("", &angles.z))
                transform->SetScale(angles);




        glm::vec3 scale = transform->GetScale();
        if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f))
        {
            transform->SetScale(scale);
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

        CameraComponent* cameraComponent = dynamic_cast<CameraComponent*>(entity->GetComponent(ComponentType::CameraComponent));
        auto& camera = cameraComponent->GetCamera();

        bool mainCamera = cameraComponent->GetPrimary();
        ImGui::Checkbox("Main Camera (button in development)", &mainCamera); // TODO there can be only one main Camera
        cameraComponent->SetPrimary(mainCamera);

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
            entity->RemoveComponent(ComponentType::CameraComponent);
    }
    void SceneHierarchyPanel::DrawMaterial(Entity* entity)
    {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

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

        MaterialComponent* material = dynamic_cast<MaterialComponent*>(entity->GetComponent(ComponentType::MaterialComponent));
        auto& shaderName = material->GetShaderName();
        auto& shaders = m_ShaderLibrary->GetShaders();
        if (ImGui::BeginCombo("Shader", shaderName.c_str()))
        {
            for (auto& [name, shader]: shaders)
            {
                bool isSelected = shaderName == name;
                if (ImGui::Selectable(name.c_str(), isSelected))
                {
                    material->SetShader(m_ShaderLibrary, name);
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        auto& textures = material->GetTextures();
        // Shader name
        ImGui::Text("Shader name: %s", material->GetShaderName().c_str());

        for (auto& [name, texture]: textures)
        {
            if (ImGui::TreeNodeEx((void*)(name.c_str()), treeNodeFlags, name.c_str()))
            {
                auto& texture2D = texture->GetTexture();
                auto rendererID = texture->GetTexture().GetRendererID();
                ImGui::Image((void*)(intptr_t)rendererID, ImVec2(64, 64), ImVec2{0.0f, 1.0f}, ImVec2{1.0f, 0.0f});
                ImGui::Text("Material name: %s", name.c_str());
                ImGui::Text("Texture Width: %d", texture2D.GetWidth());
                ImGui::Text("Texture Height: %d", texture2D.GetHeight());
                ImGui::TreePop();
            }
        }
        // Remove Component
        if (removeComponent)
            entity->RemoveComponent(ComponentType::MaterialComponent);
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
            ImGui::Begin("New mesh");
            static std::string MeshName(256, '\0');
            ImGui::InputText("Path", &MeshName[0], 256);


            if (ImGui::Button("Accept"))
            {
                createMesh = false;
                bool HasMaterial = false;
                std::shared_ptr<ShaderLibrary> shaderLibrary = nullptr;
                std::string shaderName = "undefined";
                if (m_SelectionContext->HasComponent(ComponentType::MaterialComponent))
                {
                    HasMaterial = true;
                    MaterialComponent* materialComponent = dynamic_cast<MaterialComponent*>(m_SelectionContext->GetComponent(ComponentType::MaterialComponent));
                    shaderLibrary = materialComponent->GetShaderLibrary();
                    shaderName = materialComponent->GetShaderNameCopy();
                    m_SelectionContext->RemoveComponent(ComponentType::MaterialComponent);

                }
                if (m_SelectionContext->HasComponent(ComponentType::MeshComponent))
                    m_SelectionContext->RemoveComponent(ComponentType::MeshComponent);

                if (m_SelectionContext->HasComponent(ComponentType::SubMeshComponent))
                    m_SelectionContext->RemoveComponent(ComponentType::SubMeshComponent);
                LoadMesh::CreateMesh(m_SelectionContext, MeshName);
                if (HasMaterial)
                {
                    // Add shader to material
                    MaterialComponent* materialComponent = dynamic_cast<MaterialComponent*>(m_SelectionContext->GetComponent(ComponentType::MaterialComponent));
                    materialComponent->SetShader(shaderLibrary, shaderName);
                }
            }
            ImGui::End();
        }
        auto& submeshes = dynamic_cast<MeshComponent*>(entity->GetComponent(ComponentType::MeshComponent))->GetSubMeshes();
        for (auto& subMesh: submeshes)
        {
            if (ImGui::TreeNodeEx((void*)(subMesh), treeNodeFlags, "SubMeshes"))
            {
                auto& vertexArray = subMesh->GetAttribute();
                auto& name = subMesh->GetName();

                // vertex array name
                ImGui::Text("VertexArray name: %s", name.c_str());

                // For every vertexBuffer in one VertexArray
                for (auto& vertexBuffer: vertexArray->GetVertexBuffers())
                {
                    if (ImGui::TreeNodeEx((void*)(vertexBuffer.get()), treeNodeFlags, "VertexBuffer"))
                    {
                        // Buffer array layout
                        auto& bufferLayout = vertexBuffer->GetLayout();
                        // Stride
                        ImGui::Text("Stride: %d", bufferLayout.GetStride());
                        // For evety bufferElement in one vertexBuffer
                        for (auto& bufferElement: bufferLayout.GetElements())
                        {
                            if (ImGui::TreeNodeEx((void*)(bufferElement.Name.c_str()), treeNodeFlags, bufferElement.Name.c_str()))
                            {
                                ImGui::Text("Name: %s", bufferElement.Name.c_str());
                                ImGui::Text("Offset: %d", bufferElement.Offset);
                                ImGui::Text("Size: %d", bufferElement.Size);
                                ImGui::Text("Normalized: %s", bufferElement.Normalized ? "true" : "false");
                                const char* shaderDataType[] = {
                                    "None",
                                    "Float",
                                    "Float2",
                                    "Float3",
                                    "Float4",
                                    "Mat3",
                                    "Mat4",
                                    "Int",
                                    "Int2",
                                    "Int3",
                                    "Int4",
                                    "Bool"
                                };
                                ImGui::Text("Data type: %s", shaderDataType[(int)bufferElement.Type]);
                                ImGui::TreePop();
                            }
                        }
                        ImGui::TreePop();
                    }
                }
                // Index array count
                ImGui::Text("IndexArray count: %d", vertexArray->GetIndexBuffer()->GetCount());
                ImGui::TreePop();
            }
        }

        // Remove Component
        if (removeComponent)
            entity->RemoveComponent(ComponentType::MeshComponent);
    }
}

