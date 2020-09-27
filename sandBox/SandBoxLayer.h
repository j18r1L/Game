#ifndef SandBoxLayer_H
#define SandBoxLayer_H

#include "HartEng/HartEng.h"

namespace HE
{
    class SandBoxLayer: public Layer
    {
    private:
        ShaderLibrary m_ShaderLibrary;
        std::shared_ptr<VertexArray> m_CubeVA;
        std::shared_ptr<VertexArray> m_SquareVA;
        std::shared_ptr<Texture2D> m_Texture;

        Scene *scene;
        PerspectiveCameraController m_CameraController;
    public:
        SandBoxLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep& ts) override;
        void OnImGuiRender() override;
        void OnEvent(Event &e) override;
    };
}


#endif // SandBoxLayer_H