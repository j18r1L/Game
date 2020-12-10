#include "SceneHierarchyPanel.h"

#include "HartEng/Core/pch.h"
#include "HartEng/Core/Log.h"
#include "HartEng/Core/Utils.h"

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
                    m_SelectionContext->AddComponent<CameraComponent>();
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::MenuItem("Material"))
                {
                    m_SelectionContext->AddComponent<MaterialComponent>();
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
            if (ImGui::TreeNodeEx((void*)(entity->GetComponent<TransformComponent>()), treeNodeFlags, "Transform Component"))
            {
                DrawTransform(entity);
                ImGui::TreePop();
            }
        if (entity->HasComponent<MaterialComponent>())
            if (ImGui::TreeNodeEx((void*)(entity->GetComponent<MaterialComponent>()), treeNodeFlags, "Material Component"))
            {
                DrawMaterial(entity);
                ImGui::TreePop();
            }
        if (entity->HasComponent<MeshComponent>())
            if (ImGui::TreeNodeEx((void*)(entity->GetComponent<MeshComponent>()), treeNodeFlags, "Mesh Component"))
            {
                DrawMesh(entity);
                ImGui::TreePop();
            }
        if (entity->HasComponent<CameraComponent>())
            if (ImGui::TreeNodeEx((void*)(entity->GetComponent<CameraComponent>()), treeNodeFlags, "Camera Component"))
            {
                DrawCamera(entity);
                ImGui::TreePop();
            }
        if (entity->HasComponent<LightComponent>())
            if (ImGui::TreeNodeEx((void*)(entity->GetComponent<LightComponent>()), treeNodeFlags, "Light Component"))
            {
                DrawLight(entity);
                ImGui::TreePop();
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
    void SceneHierarchyPanel::DrawMaterial(Entity* entity)
    {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

        // Remove Component
        ImGui::SameLine();
        if (ImGui::Button("+"))
            ImGui::OpenPopup("Component Settings");

        bool removeComponent = false;
        static bool addShader = false;
        if (ImGui::BeginPopup("Component Settings"))
        {
            if (ImGui::MenuItem("Remove Component"))
            {
                removeComponent = true;
            }
            if (ImGui::MenuItem("Add Shader"))
            {
                addShader = true;
            }
            ImGui::EndPopup();
        }

        MaterialComponent* material = entity->GetComponent<MaterialComponent>();
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
        // Add new shader
        if (addShader)
        {
#ifdef HE_PLATFORM_WINDOWS
            addShader = false;
            std::string filepath = FileDialog::OpenFile("Shader (*.glsl)\0*.glsl\0 ");
            if (!filepath.empty())
            {
                std::string project_path = CMAKE_PATH;
                filepath = filepath.substr(project_path.length(), filepath.length());
                auto shader = m_ShaderLibrary->Load(filepath);
                material->SetShader(m_ShaderLibrary, shader->GetName());
            }
#elif HE_PLATFORM_LINUX
            ImGui::Begin("New Shader");
            static std::string ShaderName(256, '\0');
            std::string path_to_project = CMAKE_PATH;
            ImGui::Text(path_to_project.c_str());
            ImGui::SameLine();
            ImGui::InputText("", &ShaderName[0], 256);


            if (ImGui::Button("Accept"))
            {
                addShader = false;
                auto shader = m_ShaderLibrary->Load(ShaderName);
                material->SetShader(m_ShaderLibrary, shader->GetName());
            }
            ImGui::End();
#endif
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
            entity->RemoveComponent<MaterialComponent>();

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
#ifdef HE_PLATFORM_WINDOWS
            createMesh = false;
            std::string filepath = FileDialog::OpenFile("Mesh (*.obj)\0*.obj\0(*.fbx)\0*.fbx\0 ");
            if (!filepath.empty())
            {
                std::string project_path = CMAKE_PATH;
                filepath = filepath.substr(project_path.length(), filepath.length());
                bool HasMaterial = false;
                std::shared_ptr<ShaderLibrary> shaderLibrary = nullptr;
                std::string shaderName = "undefined";
                if (m_SelectionContext->HasComponent<MaterialComponent>())
                {
                    HasMaterial = true;
                    MaterialComponent* materialComponent = m_SelectionContext->GetComponent<MaterialComponent>();
                    shaderLibrary = materialComponent->GetShaderLibrary();
                    shaderName = materialComponent->GetShaderNameCopy();
                    m_SelectionContext->RemoveComponent<MaterialComponent>();

                }
                if (m_SelectionContext->HasComponent<MeshComponent>())
                    m_SelectionContext->RemoveComponent<MeshComponent>();

                if (m_SelectionContext->HasComponent<SubMeshComponent>())
                    m_SelectionContext->RemoveComponent<SubMeshComponent>();
                if (LoadMesh::CreateMesh(m_SelectionContext, filepath))
                {
                    if (HasMaterial)
                    {
                        // Add shader to material
                        MaterialComponent* materialComponent = m_SelectionContext->GetComponent<MaterialComponent>();
                        materialComponent->SetShader(shaderLibrary, shaderName);
                    }
                }
            }
#elif HE_PLATFORM_LINUX
            ImGui::Begin("New mesh");
            static std::string MeshName(256, '\0');
            std::string path_to_project = CMAKE_PATH;
            ImGui::Text(path_to_project.c_str());
            ImGui::SameLine();
            ImGui::InputText("", &MeshName[0], 256);


            if (ImGui::Button("Accept"))
            {
                createMesh = false;
                bool HasMaterial = false;
                std::shared_ptr<ShaderLibrary> shaderLibrary = nullptr;
                std::string shaderName = "undefined";
                if (m_SelectionContext->HasComponent<MaterialComponent>())
                {
                    HasMaterial = true;
                    MaterialComponent* materialComponent = m_SelectionContext->GetComponent<MaterialComponent>();
                    shaderLibrary = materialComponent->GetShaderLibrary();
                    shaderName = materialComponent->GetShaderNameCopy();
                    m_SelectionContext->RemoveComponent<MaterialComponent>();

                }
                if (m_SelectionContext->HasComponent<MeshComponent>())
                    m_SelectionContext->RemoveComponent<MeshComponent>();

                if (m_SelectionContext->HasComponent<SubMeshComponent>())
                    m_SelectionContext->RemoveComponent<SubMeshComponent>();
                if (LoadMesh::CreateMesh(m_SelectionContext, MeshName))
                {
                    if (HasMaterial)
                    {
                        // Add shader to material
                        MaterialComponent* materialComponent = m_SelectionContext->GetComponent<MaterialComponent>();
                        materialComponent->SetShader(shaderLibrary, shaderName);
                    }
                }

            }
            ImGui::End();
#endif
        }
        if (entity->HasComponent<MeshComponent>())
        {
            auto& submeshes = entity->GetComponent<MeshComponent>()->GetSubMeshes();
            for (auto& subMesh : submeshes)
            {
                if (ImGui::TreeNodeEx((void*)(subMesh), treeNodeFlags, "SubMeshes"))
                {
                    auto& vertexArray = subMesh->GetAttribute();
                    auto& name = subMesh->GetName();

                    // vertex array name
                    ImGui::Text("VertexArray name: %s", name.c_str());

                    // For every vertexBuffer in one VertexArray
                    for (auto& vertexBuffer : vertexArray->GetVertexBuffers())
                    {
                        if (ImGui::TreeNodeEx((void*)(vertexBuffer.get()), treeNodeFlags, "VertexBuffer"))
                        {
                            // Buffer array layout
                            auto& bufferLayout = vertexBuffer->GetLayout();
                            // Stride
                            ImGui::Text("Stride: %d", bufferLayout.GetStride());
                            // For evety bufferElement in one vertexBuffer
                            const ImGuiTreeNodeFlags treeNodeFlagsBufferElement = ImGuiTreeNodeFlags_AllowItemOverlap;
                            for (auto& bufferElement : bufferLayout.GetElements())
                            {
                                if (ImGui::TreeNodeEx((void*)(bufferElement.Name.c_str()), treeNodeFlagsBufferElement, bufferElement.Name.c_str()))
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

    Entity* SceneHierarchyPanel::GetSelectedEntity() const
    {
        return m_SelectionContext;
    }
}

