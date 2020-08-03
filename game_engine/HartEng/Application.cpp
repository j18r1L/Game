#include "Application.h"

#include "HartEng/Log.h"
#include "HartEng/Input.h"

namespace HE
{

    Application* Application::s_Instance = nullptr;

    Application::Application(): m_Running(true)
    {
        HE_CORE_ASSERT(s_Instance, "Application already exists!");
        s_Instance = this;
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(HE_BIND_EVENT_FN(Application::OnEvent));

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

        HE_CORE_TRACE("{0}", e);

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
        /*
        WindowResizeEvent e(1280, 720);
        if (e.IsInCategory(EventCategoryApplication))
            HE_TRACE(e);
        if (!e.IsInCategory(EventCategoryInput))
            HE_TRACE(false);
        */

        while(m_Running)
        {
            // При вызове OnUpdate слои будут отправлять данные в отдельный поток рендерера
            for (Layer* layer: m_LayerStack)
                layer->OnUpdate();
            //auto[x, y] = Input::GetMousePosition();
            //HE_CORE_TRACE("{0}, {1}", x, y);

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


