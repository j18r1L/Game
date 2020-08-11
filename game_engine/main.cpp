#include <HartEng.h>

#include <iostream>
#include <glm/glm.hpp>

class ExampleLayer: public HE::Layer
{
private:
    std::shared_ptr<HE::Shader> m_Shader;
    std::shared_ptr<HE::VertexArray> m_VertexArray;
    std::shared_ptr<HE::VertexArray> m_SquareVA;
    HE::OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition;
    float m_CameraSpeed;

public:
    ExampleLayer():
        Layer("Example"),
        m_Camera(-1.0f, 1.0f, -1.0f, 1.0f),
        m_CameraPosition(0.0f, 0.0f, 0.0f),
        m_CameraSpeed(0.01f)
    {
        float vertices[3 * 7] = {
            -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

        };

        //Vertex array
        m_VertexArray.reset(HE::VertexArray::Create());
        m_VertexArray->Bind();
        std::shared_ptr<HE::VertexBuffer> triangleVB;
        triangleVB.reset(HE::VertexBuffer::Create(vertices, sizeof(vertices)));
        triangleVB->Bind();

        HE::BufferLayout layout = {
            {HE::ShaderDataType::Float3, "a_Position"},
            {HE::ShaderDataType::Float4, "a_Color"}
        };

        triangleVB->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(triangleVB);


        unsigned int indices[3] = {0, 1, 2};
        std::shared_ptr<HE::IndexBuffer> triangleIB;
        triangleIB.reset(HE::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(triangleIB);



        std::string vertexSrc = R"(
                #version 430 core

                layout (location = 0) in vec3 a_Position;
                layout (location = 1) in vec4 a_Color;

                uniform mat4 u_ProjectionView;

                out vec4 f_Color;

                void main()
                {
                    f_Color = vec4(1., 0., 0., 1.);
                    f_Color = a_Color;
                    gl_Position = u_ProjectionView * vec4(a_Position, 1.0);
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

        m_Shader.reset(new HE::Shader(vertexSrc, fragmentSrc));

        float vertices_square[3 * 4] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
        };

        //Vertex array
        m_SquareVA.reset(HE::VertexArray::Create());
        m_SquareVA->Bind();
        // Vertex buffer
        std::shared_ptr<HE::VertexBuffer> squareVB;
        squareVB.reset(HE::VertexBuffer::Create(vertices_square, sizeof(vertices_square)));
        squareVB->Bind();

        squareVB->SetLayout({
                                {HE::ShaderDataType::Float3, "a_Position"}
                            });
        m_SquareVA->AddVertexBuffer(squareVB);
        unsigned int indices_square[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<HE::IndexBuffer> squareIB;
        squareIB.reset(HE::IndexBuffer::Create(indices_square, sizeof(indices_square) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);

    }

    void OnUpdate() override
    {
        if (HE::Input::IsKeyPressed(HE_KEY_W))
        {
            m_CameraPosition.y += m_CameraSpeed;
        }
        if (HE::Input::IsKeyPressed(HE_KEY_S))
        {
            m_CameraPosition.y -= m_CameraSpeed;
        }
        if (HE::Input::IsKeyPressed(HE_KEY_A))
        {
            m_CameraPosition.x -= m_CameraSpeed;
        }
        if (HE::Input::IsKeyPressed(HE_KEY_D))
        {
            m_CameraPosition.x += m_CameraSpeed;
        }
        m_Camera.SetPosition(m_CameraPosition);
        HE::RenderCommand::SetClearColor(glm::vec4(0., 0., 0., 1.0));
        HE::RenderCommand::Clear();
        //Можно команды для каждой сцены разделать {}, это просто для вида :)
        //Renderer::BeginScene(camera, lights, environment);
        m_Camera.SetRotation(0.01f, glm::vec3(0.0f, 0.0f, 1.0f));
        HE::Renderer::BeginScene(m_Camera);
        {
            HE::Renderer::Submit(m_Shader, m_SquareVA);
            HE::Renderer::Submit(m_Shader, m_VertexArray);
        }
        HE::Renderer::EndScene();
        //Renderer::Flush();

    }

    void OnImGuiRender() override
    {

    }

    void OnEvent(HE::Event &event) override
    {
        //HE::EventDispatcher dispatcher(event);
        //dispatcher.Dispatch<HE::KeyPressedEvent>(HE_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
    }

    bool OnKeyPressedEvent(HE::KeyPressedEvent& event)
    {
        /*
        if (event.GetKeyCode() == HE_KEY_W)
        {
            m_CameraPosition.y += m_CameraSpeed;
        }
        if (event.GetKeyCode() == HE_KEY_S)
        {
            m_CameraPosition.y -= m_CameraSpeed;
        }
        if (event.GetKeyCode() == HE_KEY_A)
        {
            m_CameraPosition.x -= m_CameraSpeed;
        }
        if (event.GetKeyCode() == HE_KEY_D)
        {
            m_CameraPosition.x += m_CameraSpeed;
        }
        */
        return false;
    }

};

class SandBox: public HE::Application
{
public:
    SandBox()
    {
        PushLayer(new ExampleLayer());
    }

    ~SandBox() {}
};


HE::Application* HE::CreateApplication()
{
    return new SandBox();
    
}


/*
m for members
c for constants/readonlys
p for pointer (and pp for pointer to pointer)
v for volatile
s for static
i for indexes and iterators
e for events
*/
