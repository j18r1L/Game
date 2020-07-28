#include "Application.h"

#include "HartEng/Events/ApplicationEvent.h"
#include "HartEng/Log.h"

namespace HE
{
    Application::Application(): m_Running(true)
    {
        m_Window = std::unique_ptr<Window>(Window::Create());
    }

    Application::~Application()
    {

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


