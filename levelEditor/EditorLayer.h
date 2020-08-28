#ifndef EditorLayer_H
#define EditorLayer_H

#include "HartEng/HartEng.h"

namespace HE
{
    class EditorLayer: public Layer
    {
    private:
        ShaderLibrary m_ShaderLibrary;
        std::shared_ptr<VertexArray> m_CubeVA;
        std::shared_ptr<VertexArray> m_SquareVA;
        std::shared_ptr<Texture2D> m_Texture;
        std::shared_ptr<FrameBuffer> m_FrameBuffer;

        PerspectiveCameraController m_CameraController;

        // Viewport Size
        glm::vec2 m_ViewportSize;
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
