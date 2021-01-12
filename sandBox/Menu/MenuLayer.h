#ifndef MENULAYER_H
#define MENULAYER_H


#include "HartEng/HartEng.h"

// This class handles all menu UI
// Main menu
// Settings
// Pause menu
// Achievements
class MenuLayer : public HE::Layer
{
private:
    std::shared_ptr<HE::Scene> m_Scene; // This scene has all buttons 
    std::shared_ptr<HE::ShaderLibrary> m_ShaderLibrary;
    

    bool OnWindowResized(HE::WindowResizeEvent& e);
public:
    MenuLayer(std::shared_ptr<HE::ShaderLibrary> shaderLibrary);

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(HE::Timestep& ts) override;
    //void OnImGuiRender() override;
    void OnEvent(HE::Event& e) override;
};

#endif //MENULAYER_H