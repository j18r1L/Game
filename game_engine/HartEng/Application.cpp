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

        
        
        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

        };

        //Vertex array
        m_VertexArray.reset(VertexArray::Create());
        m_VertexArray->Bind();
        std::shared_ptr<VertexBuffer> triangleVB;
        triangleVB.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
        triangleVB->Bind();

        BufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"}
        };

        triangleVB->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(triangleVB);


        unsigned int indices[3] = {0, 1, 2};
        std::shared_ptr<IndexBuffer> triangleIB;
        triangleIB.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(triangleIB);



        std::string vertexSrc = R"(
                #version 430 core

                layout (location = 0) in vec3 a_Position;
                layout (location = 1) in vec4 a_Color;

                out vec4 f_Color;

                void main()
                {
                    f_Color = vec4(1., 0., 0., 1.);
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

        m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

        float vertices_square[3 * 4] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
        };

        //Vertex array
        m_SquareVA.reset(VertexArray::Create());
        m_SquareVA->Bind();
        // Vertex buffer
        std::shared_ptr<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(vertices_square, sizeof(vertices_square)));
        squareVB->Bind();

        squareVB->SetLayout({
                                {ShaderDataType::Float3, "a_Position"}
                            });
        m_SquareVA->AddVertexBuffer(squareVB);
        unsigned int indices_square[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(indices_square, sizeof(indices_square) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);


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
        RenderCommand::SetClearColor(glm::vec4(0., 0., 0., 1.0));
        while(m_Running)
        {

            RenderCommand::Clear();
            //Можно команды для каждой сцены разделать {}, это просто для вида :)
            //Renderer::BeginScene(camera, lights, environment);
            //{

            //}
            Renderer::BeginScene();
            m_Shader->Bind();
            Renderer::Submit(m_SquareVA);
            Renderer::Submit(m_VertexArray);

            Renderer::EndScene();

            //Renderer::Flush();



            //m_SquareVA->Bind();
            //glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

            //m_VertexArray->Bind();
            //glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);



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


