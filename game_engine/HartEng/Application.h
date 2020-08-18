#ifndef APPLICATION_H
#define APPLICATION_H

#include "HartEng/pch.h"

#include "HartEng/Keycodes.h"
#include "HartEng/MouseButtonCodes.h"
#include "HartEng/Core.h"
#include "HartEng/LayerStack.h"
#include "HartEng/Window.h"

#include "HartEng/Events/Event.h"
#include "HartEng/Events/ApplicationEvent.h"

#include "HartEng/imGUI/imGUILayer.h"

#include "HartEng/Core/Timestep.h"

#include "HartEng/Renderer/Buffer.h"
#include "HartEng/Renderer/Shader.h"
#include "HartEng/Renderer/VertexArray.h"
#include "HartEng/Renderer/Renderer.h"
#include "HartEng/Renderer/Cameras.h"

namespace HE
{
    class Application
    {
    private:
        std::unique_ptr<Window> m_Window;

        ImGUILayer* m_ImGuiLayer;

        LayerStack m_LayerStack;

        Timestep m_Timestep;
        float m_CurrentTime;



        static Application* s_Instance;

        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent &e);
    public:
        bool m_Running = true; // Gameloop
        bool m_Minimized = false;
        Application();
        virtual ~Application();
        void Run();
        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        inline Window& GetWindow()
        {
            return *m_Window;
        }
        static inline Application& Get()
        {
            return *s_Instance;
        }
    };

    // To be defined in a client
    Application* CreateApplication();

}

#endif // APPLICATION_H
