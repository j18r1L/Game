#include "SandBoxLayer.h"



SandBoxLayer::SandBoxLayer(std::shared_ptr<HE::ShaderLibrary> shaderLibrary):
    Layer("SandBoxLayer"),
    m_ShaderLibrary(shaderLibrary)
{
    m_Scene = std::make_shared<HE::Scene>(HE::Scene("first_scene"));
    HE::RenderCommand::SetClearColor(glm::vec4(0.5, 0., 0.5, 1.0));


}

void SandBoxLayer::OnAttach()
{
    HE_PROFILE_FUNCTION();

    std::string path_to_project = CMAKE_PATH;
    HE::SceneSerializer serializer(m_Scene, m_ShaderLibrary);
    serializer.Deserialize(path_to_project + "/assets/scenes/scene.he");
    

}

void SandBoxLayer::OnDetach()
{

}

void SandBoxLayer::OnUpdate(HE::Timestep& ts)
{
    HE_PROFILE_FUNCTION();


    {
        HE_PROFILE_SCOPE("Render prep");

        // Renderer
        //HE::RenderCommand::Clear();
    }

    {
        HE_PROFILE_SCOPE("Renderer Draw");

        
        HE::RenderCommand::SetDepthTest(true);
        m_Scene->OnUpdate(ts);
    }
}

void SandBoxLayer::OnImGuiRender()
{
    HE_PROFILE_FUNCTION();



}

void SandBoxLayer::OnEvent(HE::Event &e)
{
    HE_PROFILE_FUNCTION();
    HE::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<HE::WindowResizeEvent>(HE_BIND_EVENT_FN(SandBoxLayer::OnWindowResized));
}

bool SandBoxLayer::OnWindowResized(HE::WindowResizeEvent& e)
{
    m_Scene->OnViewportResize(e.GetWidth(), e.GetHeight());
    return false;
}



