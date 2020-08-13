#include <HartEng.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

class ExampleLayer: public HE::Layer
{
private:
    std::shared_ptr<HE::Shader> m_Shader;
    std::shared_ptr<HE::VertexArray> m_VertexArray;
    std::shared_ptr<HE::VertexArray> m_SquareVA;
    std::shared_ptr<HE::Texture2D> m_Texture;

    HE::OrthographicCamera m_Camera;

    glm::vec3 m_SquarePosition;
    float m_SquareSpeed;
    glm::vec3 m_SquareColor;

public:
    ExampleLayer():
        Layer("Example"),
        m_Camera(-1.0f, 1.0f, -1.0f, 1.0f),
        m_SquarePosition(0.0f),
        m_SquareSpeed(1.0f)
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

        m_Shader.reset(HE::Shader::Create("../assets/shaders/Texture.glsl"));
        //m_Shader.reset(m_Shader->Create(vertexSrc, fragmentSrc));

        float vertices_square[5 * 4] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        };

        //Vertex array
        m_SquareVA.reset(HE::VertexArray::Create());
        m_SquareVA->Bind();
        // Vertex buffer
        std::shared_ptr<HE::VertexBuffer> squareVB;
        squareVB.reset(HE::VertexBuffer::Create(vertices_square, sizeof(vertices_square)));
        squareVB->Bind();

        squareVB->SetLayout({
            {HE::ShaderDataType::Float3, "a_Position"},
            {HE::ShaderDataType::Float2, "a_TexCoord"}
        });
        m_SquareVA->AddVertexBuffer(squareVB);
        unsigned int indices_square[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<HE::IndexBuffer> squareIB;
        squareIB.reset(HE::IndexBuffer::Create(indices_square, sizeof(indices_square) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);

        HE::RenderCommand::SetClearColor(glm::vec4(0., 0., 0., 1.0));

        // load texture
        m_Texture = HE::Texture2D::Create("../media/red.png");
        m_Shader->Bind();
        m_Shader->SetInt("u_Texture", 0);
        m_Texture->Bind(0);

    }

    void OnUpdate(HE::Timestep& ts) override
    {
        float deltaTime = ts;
        // OnUpdate выполняется на каждый кадр, здесь следует посредством Input polling-а обрабатывать движения, стрельбу и другие игровые действия, требующие не единичного использования
        // Например подобрать предмет нажатием на E лучше запихнуть в эвент систему
        // так, допустим, нажатие мыши имеет только евенты КнопкаМышиНажата и КнопкаМышиОтжата, у нее нет эвента КнопкаМышиЗажата
        if (HE::Input::IsKeyPressed(HE_KEY_W))
        {
            m_SquarePosition.y += m_SquareSpeed * deltaTime;
        }
        if (HE::Input::IsKeyPressed(HE_KEY_S))
        {
            m_SquarePosition.y -= m_SquareSpeed * deltaTime;
        }
        if (HE::Input::IsKeyPressed(HE_KEY_A))
        {
            m_SquarePosition.x -= m_SquareSpeed * deltaTime;
        }
        if (HE::Input::IsKeyPressed(HE_KEY_D))
        {
            m_SquarePosition.x += m_SquareSpeed * deltaTime;
        }

        glm::mat4 square_transform = glm::translate(glm::mat4(1.0f), m_SquarePosition);


        HE::RenderCommand::Clear();
        //Можно команды для каждой сцены разделать {}, это просто для вида :)
        //Renderer::BeginScene(camera, lights, environment);
        m_Camera.SetRotation(0.01f, glm::vec3(0.0f, 0.0f, 1.0f));
        HE::Renderer::BeginScene(m_Camera);
        {
            // Из шейдера достаем все нужные юниформы, Material - буффер, который хранит информацию о материале, заполняет ее стандартными значениям
            //HE::Material* material = new HE::Material(m_Shader);


            // Настраиваем instance материала, пример: base material - серебро, но мы делаем material_instance->SetTexture("u_Roughness", roughness_map_texture); - устанавливаем индивидуально для каждой instance свою карту шереховатости
            //HE::MaterialInstance* material_instance = new HE::MaterialInstance(material);
            //material_instance->SetValue("u_Color", redColor);
            //mesh->SetMaterial(material_instance);
            //HE::Renderer::Submit(m_Shader, m_SquareVA, square_transform);
            m_Shader->Bind();
            m_Shader->SetVec3("u_Color", m_SquareColor);

            HE::Renderer::Submit(m_Shader, m_SquareVA, square_transform);
        }
        HE::Renderer::EndScene();
        //Renderer::Flush();

    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Setting");

        ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

        ImGui::End();
    }

    void OnEvent(HE::Event &event) override
    {
        // Вот таким образом добавляем функцию обработчик эвентов, который может быть у каждого слоя свой
        //HE::EventDispatcher dispatcher(event);
        //dispatcher.Dispatch<HE::KeyPressedEvent>(HE_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
    }

    bool OnKeyPressedEvent(HE::KeyPressedEvent& event)
    {
        // описываем функцию обработчика эвента, ее в целом надо использовать в менюшках, нажатиях кнопки мыши в них, работы с инвентарем, но не для стрельбы, ходьбы
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
