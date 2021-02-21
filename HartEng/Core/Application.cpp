#include "Application.h"

#include "HartEng/Core/Log.h"
#include "HartEng/Core/Input.h"
#include "HartEng/Physics/Physics.h"

namespace HE
{

    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name):
        m_Window(std::unique_ptr<Window>(Window::Create(name))),
        m_ImGuiLayer(new ImGUILayer()),
        m_Running(true)
    {
        HE_CORE_ASSERT(!s_Instance, "Application already exists!");

        HE_PROFILE_FUNCTION();
        s_Instance = this;
#ifdef HE_PLATFORM_WINDOWS
        HE_CORE_INFO("Windows platform");
#elif HE_PLATFORM_LINUX
        HE_CORE_INFO("Linux platform");
#endif

        m_Window->SetEventCallback(HE_BIND_EVENT_FN(Application::OnEvent));
        Renderer::Init();
        Physics::Init();
        Renderer::WaitAndRender();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
        Renderer::Shutdown();
        Physics::Shutdown();
    }

    void Application::PushLayer(Layer* layer)
    {
        HE_PROFILE_FUNCTION();

        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }
    void Application::PushOverlay(Layer* overlay)
    {
        HE_PROFILE_FUNCTION();

        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::OnEvent(Event& e)
    {
        HE_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(HE_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(HE_BIND_EVENT_FN(Application::OnWindowResize));
#ifdef HE_TRACE_EVENTS
        HE_CORE_TRACE("{0}", e);
#endif

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    void Application::OnRenderImGui()
    {
        m_ImGuiLayer->Begin();
        {
            HE_PROFILE_SCOPE("LayerStack::OnImGuiRender");
            for (Layer* layer : m_LayerStack)
            {
                layer->OnImGuiRender();
            }
        }
        m_ImGuiLayer->End();
    }

    void Application::Close()
    {
        m_Running = false;
    }

    void Application::Run()
    {
        HE_PROFILE_FUNCTION();

        // TODO сделать хороший timestep из Fix your time step
        m_CurrentTime = m_Window->GetTime();

        while(m_Running)
        {
            HE_PROFILE_SCOPE("RunLoop");

            float time = m_Window->GetTime();
            m_Timestep.SetTime(time - m_CurrentTime);
            m_CurrentTime = time;

            if (!m_Minimized)
            {
                {
                    HE_PROFILE_SCOPE("LayerStack::OnUpdate");
                    for (Layer* layer: m_LayerStack)
                        layer->OnUpdate(m_Timestep);
                }
                Application* app = this;
                Renderer::Submit([app]() 
                    { 
                        app->OnRenderImGui();
                    });
                Renderer::WaitAndRender();
                
            }
            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent &e)
    {
        HE_PROFILE_FUNCTION();

        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent &e)
    {
        HE_PROFILE_FUNCTION();

        uint32_t width = e.GetWidth();
        uint32_t height = e.GetHeight();
        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }
        m_Minimized = false;

        Renderer::OnWindowResize(width, height);
        return false;
    }
}


