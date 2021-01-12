#include <HartEng/HartEng.h>


// Entry point
#include "HartEng/Core/EntryPoint.h"

#include "SandBoxLayer.h"
#include "Menu/MenuLayer.h"

class SandBox: public HE::Application
{
private:
    std::shared_ptr<HE::ShaderLibrary> m_ShaderLibrary;
public:
    SandBox():
        HE::Application("Game")
    {
        m_ShaderLibrary = std::make_shared<HE::ShaderLibrary>(HE::ShaderLibrary());
        PushOverlay(new MenuLayer(m_ShaderLibrary));
        PushLayer(new SandBoxLayer(m_ShaderLibrary));
    }

    ~SandBox() {}
};


HE::Application* HE::CreateApplication()
{
    return new SandBox();

}



