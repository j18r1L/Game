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

        //Vertex array
        //vertex buffer
        //index buffer
        //shader
        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);

        float vertices[3 * 3] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f,

        };

        m_VertexBuffer = std::make_unique<VertexBuffer> (VertexBuffer::Create(vertices, sizeof(vertices)));
        m_VertexBuffer->Bind();

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);



        unsigned int indices[3] = {0, 1, 2};
        m_IndexBuffer = std::make_unique<IndexBuffer> (IndexBuffer::Create(indices, sizeof(indices)));

        std::string vertexSrc = R"(
                #version 430 core

                layout (location = 0) in vec3 a_Position;

                void main()
                {
                    gl_Position = vec4(a_Position, 1.0);
                }


            )";

        std::string fragmentSrc = R"(
                #version 430 core

                layout (location = 0) out vec4 o_Color;

                void main()
                {
                    o_Color = vec4(1., 0, 0, 1.0);
                }


            )";

        m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

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
        /*
        WindowResizeEvent e(1280, 720);
        if (e.IsInCategory(EventCategoryApplication))
            HE_TRACE(e);
        if (!e.IsInCategory(EventCategoryInput))
            HE_TRACE(false);
        */
        glClearColor(0., 0., 0., 1.0);
        while(m_Running)
        {

            glClear(GL_COLOR_BUFFER_BIT);

            m_Shader->Bind();
            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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


