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
        m_CurrentTime = m_Window->GetTime();

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
        dispatcher.Dispatch<WindowCloseEvent>(HE_BIND_EVENT_FN(Application::OnWindowClosed));
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

    bool Application::OnWindowClosed(WindowCloseEvent &e)
    {
        m_Running = false;
        return true;
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

            // При вызове OnUpdate слои будут отправлять данные в отдельный поток рендерера
            for (Layer* layer: m_LayerStack)
                layer->OnUpdate(m_Timestep);

            // TODO когда напишу рендерер, то эта часть будет на отдельном потоке
            m_ImGuiLayer->Begin();
            for (Layer* layer: m_LayerStack)
            {
                layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();

            m_Window->OnUpdate();

        }
    }
}


