#include "Application.h"

#include "HartEng/Log.h"
#include "HartEng/Input.h"

namespace HE
{

    Application* Application::s_Instance = nullptr;

    Application::Application() : m_Running(true)
    {
        HE_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(HE_BIND_EVENT_FN(Application::OnEvent));

        m_ImGuiLayer = new ImGUILayer();
        PushOverlay(m_ImGuiLayer);

        //Vertex array
        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

        };


        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
        m_VertexBuffer->Bind();

        // Пример, скоро уберу, когда отделю Opengl контекс от application-а
        {
            BufferLayout layout = {
                {ShaderDataType::Float3, "a_Position"},
                {ShaderDataType::Float4, "a_Color"}
            };
            m_VertexBuffer->SetLayout(layout);
        }

        uint32_t index = 0;


        const auto& layout_1 = m_VertexBuffer->GetLayout();
        for (const auto& element : layout_1)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, element.GetComponentCount(), GL_FLOAT, element.Normalized ? GL_TRUE : GL_FALSE, layout_1.GetStride(), (void*)element.Offset);
            index++;
        }

        

        



        unsigned int indices[3] = {0, 1, 2};
        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        std::string vertexSrc = R"(
                #version 430 core

                layout (location = 0) in vec3 a_Position;
                layout (location = 1) in vec4 a_Color;

                out vec4 f_Color;

                void main()
                {
                    f_Color = a_Color;
                    gl_Position = vec4(a_Position, 1.0);
                }


            )";

        std::string fragmentSrc = R"(
                #version 430 core

                layout (location = 0) out vec4 o_Color;

                in vec4 f_Color;

                void main()
                {
                    o_Color = vec4(f_Color);
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
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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


