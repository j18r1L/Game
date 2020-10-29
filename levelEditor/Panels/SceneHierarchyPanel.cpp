#include "SceneHierarchyPanel.h"

#include "HartEng/Core/pch.h"
#include "HartEng/Core/Log.h"
#include "glm/gtc/type_ptr.hpp"

namespace HE
{
    SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<Scene>& scene)
    {
        SetScene(scene);
    }

    void SceneHierarchyPanel::SetScene(const std::shared_ptr<Scene>& scene)
    {
        m_Scene = scene;
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Scene Hierarchy");

        for (auto& [name, entity]: m_Scene->m_Entities)
        {
            DrawEntityNode(name, entity);
        }

        //Deselect entity if pressed in window, but not on a entity
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectionContext = nullptr;

        ImGui::End();

        ImGui::Begin("Properties");
        if (m_SelectionContext)
        {
            DrawComponents(m_SelectionContext);
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

        if (opened)
        {
            ImGui::Text(name.c_str());
            ImGui::TreePop();
        }
    }

    void SceneHierarchyPanel::DrawComponents(Entity* entity)
    {
        if (entity->HasComponent(ComponentType::TransformComponent))
        {
            if (ImGui::TreeNodeEx((void*)(ComponentType::TransformComponent), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
            {
                TransformComponent* transform = dynamic_cast<TransformComponent*>(entity->GetComponent(ComponentType::TransformComponent));

                glm::vec3 position = transform->GetTranslation();
                if (ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f))
                {
                    transform->SetTranslation(position);
                }

                glm::quat rotation = transform->GetRotation();
                glm::vec3 angles = glm::eulerAngles(rotation);
                if (ImGui::DragFloat3("Rotation", glm::value_ptr(angles), 0.1f))
                {
                    transform->SetRotation(angles);
                }

                glm::vec3 scale = transform->GetScale();
                if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f))
                {
                    transform->SetScale(scale);
                }

                ImGui::TreePop();
            }


        }
        if (entity->HasComponent(ComponentType::TransformComponent))
        {

        }
        if (entity->HasComponent(ComponentType::TransformComponent))
        {

        }
    }
}

