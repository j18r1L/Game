#ifndef TESTLAYER_H
#define TESTLAYER_H

#include "HartEng.h"

class TestLayer: public HE::Layer
{
private:
    HE::ShaderLibrary m_ShaderLibrary;
    std::shared_ptr<HE::VertexArray> m_CubeVA;
    std::shared_ptr<HE::VertexArray> m_SquareVA;
    std::shared_ptr<HE::Texture2D> m_Texture;
    std::shared_ptr<HE::FrameBuffer> m_FrameBuffer;

    HE::PerspectiveCameraController m_CameraController;
public:
    TestLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(HE::Timestep& ts) override;
    void OnImGuiRender() override;
    void OnEvent(HE::Event &e) override;
};

#endif // TESTLAYER_H
