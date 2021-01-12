#ifndef SandBoxLayer_H
#define SandBoxLayer_H

#include "HartEng/HartEng.h"

class SandBoxLayer: public HE::Layer
{
private:
    std::shared_ptr<HE::ShaderLibrary> m_ShaderLibrary;
    HE::Entity* environmentEntity;

    std::shared_ptr<HE::Scene> m_Scene;

    bool OnWindowResized(HE::WindowResizeEvent& e);
public:
    SandBoxLayer(std::shared_ptr<HE::ShaderLibrary> shaderLibrary);

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(HE::Timestep& ts) override;
    void OnImGuiRender() override;
    void OnEvent(HE::Event &e) override;
};


#endif // SandBoxLayer_H
