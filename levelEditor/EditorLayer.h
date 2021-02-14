#pragma once

#include "HartEng/HartEng.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Gizmo/Gizmo.h"

#include "assets/scripts/ObstacleScript.h"
#include "assets/scripts/PlayerControllerScript.h"

namespace HE
{
    class EditorLayer: public Layer
    {
    private:
        enum class SceneState
        {
            Edit = 0, Play = 1, Pause = 2
        };
        SceneState m_SceneState = SceneState::Edit;

        Entity* environmentEntity;

        std::shared_ptr<Scene> m_Scene;
        PerspectiveCameraController m_CameraController;

        // Viewport Size
        glm::vec2 m_ViewportSize;
        glm::vec2 m_ViewportBounds[2];
        bool m_ViewportFocused = true;

        // Gizmos
        Gizmo m_Gizmo;
        // Panels
        std::shared_ptr<SceneHierarchyPanel> m_SceneHierarchyPanel;

        // Event functions
        bool OnMouseButton(MouseButtonPressedEvent& event);

        void Draw(Timestep& ts);
        void OnScenePlay();
        void OnSceneStop();

    public:
        EditorLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep& ts) override;
        void OnImGuiRender() override;
        void OnEvent(Event &e) override;
    };
}







