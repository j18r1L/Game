#include "Application.h"

#include "HartEng/Events/ApplicationEvent.h"
#include "HartEng/Log.h"


namespace HE
{
    Application::Application()
    {

    }

    Application::~Application()
    {

    }

    void Application::Run()
    {
        WindowResizeEvent e(1280, 720);
        if (e.IsInCategory(EventCategoryApplication))
            HE_TRACE(e);
        if (!e.IsInCategory(EventCategoryInput))
            HE_TRACE(false);

        while(true);
    }
}


