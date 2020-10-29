#ifndef EditorLayer_H
#define EditorLayer_H

#include "HartEng/HartEng.h"
#include "Panels/SceneHierarchyPanel.h"

namespace HE
{
    class EditorLayer: public Layer
    {
    private:
        ShaderLibrary m_ShaderLibrary;
        std::shared_ptr<VertexArray> m_CubeVA;
        //std::shared_ptr<VertexArray> m_SquareVA;
        //std::shared_ptr<Texture2D> m_Texture;
        std::shared_ptr<FrameBuffer> m_FrameBuffer;

        std::shared_ptr<Scene> m_Scene;
        PerspectiveCameraController m_CameraController;

        // Viewport Size
        glm::vec2 m_ViewportSize;
        bool m_ViewportFocused = true;


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
