#include "SceneHierarchyPanel.h"

#include "HartEng/Core/pch.h"
#include "HartEng/Core/Log.h"

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
}

