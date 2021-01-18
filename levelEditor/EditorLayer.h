#ifndef EditorLayer_H
#define EditorLayer_H

#include "HartEng/HartEng.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Gizmo/Gizmo.h"

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


        std::shared_ptr<ShaderLibrary> m_ShaderLibrary;
        Entity* environmentEntity;

        FrameBufferSpecification m_FrameBufferSpec;
        std::shared_ptr<FrameBuffer> m_FrameBuffer;
        std::shared_ptr<FrameBuffer> m_FrameBuffer_msaa;
        std::shared_ptr<FrameBuffer> m_IDFrameBuffer;

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


#endif // EditorLayer_H
