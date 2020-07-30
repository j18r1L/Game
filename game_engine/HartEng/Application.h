#ifndef APPLICATION_H
#define APPLICATION_H

#include "Core.h"
#include "HartEng/Events/Event.h"
#include "HartEng/Events/ApplicationEvent.h"
#include "HartEng/LayerStack.h"
#include "HartEng/Window.h"

namespace HE
{
    class Application
    {
    private:
        std::unique_ptr<Window> m_Window;

        LayerStack m_LayerStack;

        bool OnWindowClosed(WindowCloseEvent& e);
    public:
        bool m_Running;
        Application();
        virtual ~Application();
        void Run();
        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
    };

    // To be defined in a client
    Application* CreateApplication();

}

#endif // APPLICATION_H
