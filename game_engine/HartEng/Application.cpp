#include "Application.h"

#include "HartEng/Log.h"

namespace HE
{

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

    Application::Application(): m_Running(true)
    {
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
    }

    Application::~Application()
    {

    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClosed));

        HE_CORE_TRACE("{0}", e);
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
            m_Window->OnUpdate();
        }
    }
}


