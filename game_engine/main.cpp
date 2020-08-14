#include <HartEng.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>


class ExampleLayer: public HE::Layer
{
private:
    HE::ShaderLibrary m_ShaderLibrary;
    std::shared_ptr<HE::VertexArray> m_VertexArray;
    std::shared_ptr<HE::VertexArray> m_SquareVA;
    std::shared_ptr<HE::Texture2D> m_Texture;

    HE::PerspectiveCamera m_Camera;
    //HE::OrthographicCamera m_Camera;

    glm::vec3 m_CameraPosition;
    glm::quat m_CameraRotation;
    float m_CameraSpeed;

public:
    ExampleLayer():
        Layer("Example"),
        //m_Camera(-1.0f, 1.0f, -1.0f, 1.0f),
        m_Camera(45.0f, HE::Application::Get().GetWindow().GetWidth() / HE::Application::Get().GetWindow().GetHeight(), 0.1f, 10.0f),
        m_CameraPosition(0.0f),
        m_CameraRotation(),
        m_CameraSpeed(1.0f)
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

        auto textureShader = m_ShaderLibrary.Load("../assets/shaders/Texture.glsl");

        float vertices_square[5 * 4] = {
            -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, -1.0f, 0.0f, 1.0f,
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
        textureShader->Bind();
        textureShader->SetInt("u_Texture", 0);
        m_Texture->Bind(0);

    }

    void OnUpdate(HE::Timestep& ts) override
    {
        float deltaTime = ts;
        // OnUpdate выполняется на каждый кадр, здесь следует посредством Input polling-а обрабатывать движения, стрельбу и другие игровые действия, требующие не единичного использования
        // Например подобрать предмет нажатием на E лучше запихнуть в эвент систему
        // так, допустим, нажатие мыши имеет только евенты КнопкаМышиНажата и КнопкаМышиОтжата, у нее нет эвента КнопкаМышиЗажата
        // TODO обработка камеры должна быть в компоненте камеры, заскриптованной через
        if (HE::Input::IsKeyPressed(HE_KEY_W))
        {
            m_CameraPosition.z += m_CameraSpeed * deltaTime;
        }
        if (HE::Input::IsKeyPressed(HE_KEY_S))
        {
            m_CameraPosition.z -= m_CameraSpeed * deltaTime;
        }
        if (HE::Input::IsKeyPressed(HE_KEY_A))
        {
            m_CameraPosition.x -= m_CameraSpeed * deltaTime;
        }
        if (HE::Input::IsKeyPressed(HE_KEY_D))
        {
            m_CameraPosition.x += m_CameraSpeed * deltaTime;
        }
        if (HE::Input::IsKeyPressed(HE_KEY_Q))
        {
            m_CameraPosition.y += m_CameraSpeed * deltaTime;
        }
        if (HE::Input::IsKeyPressed(HE_KEY_E))
        {
            m_CameraPosition.y -= m_CameraSpeed * deltaTime;
        }
        m_Camera.SetPosition(m_CameraPosition);
        HE_INFO("camera pos.x = {0}", m_CameraPosition.x);
        HE_INFO("camera pos.y = {0}", m_CameraPosition.y);
        HE_INFO("camera pos.z = {0}", m_CameraPosition.z);



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
            auto textureShader = m_ShaderLibrary.Get("Texture");
            textureShader->Bind();

            HE::Renderer::Submit(textureShader, m_SquareVA);
        }
        HE::Renderer::EndScene();
        //Renderer::Flush();

    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Setting");

        //ImGui::InputFloat("input scientific", &m_CameraPosition.z, 0.0, 0.0, "%e");
        //HE_INFO("camera pos.z = {0}", m_CameraPosition.z);
        //ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

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
