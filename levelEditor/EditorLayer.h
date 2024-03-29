#ifndef EditorLayer_H
#define EditorLayer_H

#include "HartEng/HartEng.h"
#include "Panels/SceneHierarchyPanel.h"

namespace HE
{
    class EditorLayer: public Layer
    {
    private:
        std::shared_ptr<ShaderLibrary> m_ShaderLibrary;
        Entity* environmentEntity;

        FrameBufferSpecification m_FrameBufferSpec;
        std::shared_ptr<FrameBuffer> m_FrameBuffer;

        std::shared_ptr<Scene> m_Scene;
        PerspectiveCameraController m_CameraController;

        // Viewport Size
        glm::vec2 m_ViewportSize;
        bool m_ViewportFocused = true;
        bool m_Play = false;
        bool m_Pause = false;


        // Panels
        std::shared_ptr<SceneHierarchyPanel> m_SceneHierarchyPanel;
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
