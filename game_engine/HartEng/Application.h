#ifndef APPLICATION_H
#define APPLICATION_H

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace HE
{
    class Application
    {
    private:
        std::unique_ptr<Window> m_Window;
    public:
        bool m_Running;
        Application();
        virtual ~Application();
        void Run();
    };

    // To be defined in a client
    Application* CreateApplication();

}

#endif // APPLICATION_H
