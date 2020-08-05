#ifndef APPLICATION_H
#define APPLICATION_H


#include "HartEng/Keycodes.h"
#include "HartEng/MouseButtonCodes.h"
#include "HartEng/Core.h"
#include "HartEng/Events/Event.h"
#include "HartEng/Events/ApplicationEvent.h"
#include "HartEng/LayerStack.h"
#include "HartEng/Window.h"
#include "HartEng/imGUI/imGUILayer.h"
#include "HartEng/Renderer/Buffer.h"
#include "HartEng/Renderer/Shader.h"


namespace HE
{
    class Application
    {
    private:
        std::unique_ptr<Window> m_Window;
        ImGUILayer* m_ImGuiLayer;

        LayerStack m_LayerStack;

        bool OnWindowClosed(WindowCloseEvent& e);

        static Application* s_Instance;

        unsigned int m_VertexArray;
        std::unique_ptr<Shader> m_Shader;

        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
    public:
        bool m_Running;
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
