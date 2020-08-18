#include "Application.h"

#include "HartEng/Log.h"
#include "HartEng/Input.h"

namespace HE
{

    Application* Application::s_Instance = nullptr;

    Application::Application() :
        m_Running(true)
    {
        HE_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;
#ifdef HE_PLATFORM_WINDOWS
        HE_CORE_INFO("Windows platform");
#elif HE_PLATFORM_LINUX
        HE_CORE_INFO("Linux platform");
#endif

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(HE_BIND_EVENT_FN(Application::OnEvent));

        Renderer::Init();

        m_ImGuiLayer = new ImGUILayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {

    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }
    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
    }

    void Application::OnEvent(Event& e)
    {
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



    void Application::Run()
    {
        // TODO сделать хороший timestep из Fix your time step
        m_CurrentTime = m_Window->GetTime();

        while(m_Running)
        {
            float time = m_Window->GetTime();
            m_Timestep.SetTime(time - m_CurrentTime);
            m_CurrentTime = time;

            if (!m_Minimized)
            {
                for (Layer* layer: m_LayerStack)
                    layer->OnUpdate(m_Timestep);
            }


            m_ImGuiLayer->Begin();
            for (Layer* layer: m_LayerStack)
            {
                layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();

            m_Window->OnUpdate();

        }
    }

    bool Application::OnWindowClose(WindowCloseEvent &e)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent &e)
    {
        uint32_t width = e.GetWidth();
        uint32_t height = e.GetHeight();
        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }
        m_Minimized = false;

        Renderer::OnWindowResize(width, height);
        return true;
    }
}


