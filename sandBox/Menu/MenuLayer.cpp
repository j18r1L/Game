#include "MenuLayer.h"

MenuLayer::MenuLayer(std::shared_ptr<HE::ShaderLibrary> shaderLibrary):
	Layer("MenuLayer"),
	m_ShaderLibrary(shaderLibrary)
{
	m_Scene = std::make_shared<HE::Scene>(HE::Scene("Menu"));
}

void MenuLayer::OnAttach()
{
	std::string path_to_project = CMAKE_PATH;
	HE::SceneSerializer serializer(m_Scene, m_ShaderLibrary);
	serializer.Deserialize(path_to_project + "/assets/scenes/menu.he");
	m_Scene->OnViewportResize(1, 1);
}

void MenuLayer::OnDetach()
{

}

void MenuLayer::OnUpdate(HE::Timestep& ts)
{
	HE::RenderCommand::SetDepthTest(false);
	m_Scene->OnUpdate(ts);
}

void MenuLayer::OnEvent(HE::Event& e)
{
	HE_PROFILE_FUNCTION();
	HE::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<HE::WindowResizeEvent>(HE_BIND_EVENT_FN(MenuLayer::OnWindowResized));
}

bool MenuLayer::OnWindowResized(HE::WindowResizeEvent& e)
{
	//m_Scene->OnViewportResize(e.GetWidth(), e.GetHeight());
	return false;
}