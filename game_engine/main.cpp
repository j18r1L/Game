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

    //HE::OrthographicCameraController m_CameraController;
    HE::PerspectiveCameraController m_CameraController;

public:
    ExampleLayer():
        Layer("Example"),
        //m_CameraController(HE::Application::Get().GetWindow().GetWidth() / HE::Application::Get().GetWindow().GetHeight())
        m_CameraController(45.0f, HE::Application::Get().GetWindow().GetWidth() / HE::Application::Get().GetWindow().GetHeight(), 0.1f, 10.0f)
    {
        m_CameraController.SetPosition({-1.0f, 0.0f, 0.0f});
        m_CameraController.SetRotation(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));

        auto textureShader = m_ShaderLibrary.Load("../assets/shaders/Texture.glsl");

        float vertices_square[5 * 4] = {
            0.0f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.0f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.0f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.0f, 0.5f, -0.5f, 0.0f, 1.0f,
        };

        //Vertex array
        m_SquareVA = HE::VertexArray::Create();
        m_SquareVA->Bind();
        // Vertex buffer
        std::shared_ptr<HE::VertexBuffer> squareVB;
        squareVB = HE::VertexBuffer::Create(vertices_square, sizeof(vertices_square));
        squareVB->Bind();

        squareVB->SetLayout({
            {HE::ShaderDataType::Float3, "a_Position"},
            {HE::ShaderDataType::Float2, "a_TexCoord"}
        });
        m_SquareVA->AddVertexBuffer(squareVB);
        unsigned int indices_square[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<HE::IndexBuffer> squareIB;
        squareIB = HE::IndexBuffer::Create(indices_square, sizeof(indices_square) / sizeof(uint32_t));
        m_SquareVA->SetIndexBuffer(squareIB);

        HE::RenderCommand::SetClearColor(glm::vec4(0., 0., 0., 1.0));

        // load texture
        m_Texture = HE::Texture2D::Create("../media/tex_coord.png");
        textureShader->Bind();
        textureShader->SetInt("u_Texture", 0);
        m_Texture->Bind(0);

    }

    void OnUpdate(HE::Timestep& ts) override
    {

        // Update
        m_CameraController.OnUpdate(ts);
        // Renderer
        HE::RenderCommand::Clear();
        HE::Renderer::BeginScene(m_CameraController.GetCamera());
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
        //ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

        ImGui::End();
    }

    void OnEvent(HE::Event &e) override
    {
        m_CameraController.OnEvent(e);
    }

    bool OnKeyPressedEvent(HE::KeyPressedEvent& e)
    {
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
