#include "SandBoxLayer.h"


namespace HE
{
    SandBoxLayer::SandBoxLayer():
        Layer("SandBoxLayer"),
        m_CameraController(45.0f, Application::Get().GetWindow().GetWidth() / Application::Get().GetWindow().GetHeight(), 0.1f, 100.0f)
    {
        m_Scene = std::make_unique<Scene>(Scene("first_scene"));
        //*m_Scene = Scene("first scene");
    }

    void SandBoxLayer::OnAttach()
    {
        HE_PROFILE_FUNCTION();

        m_CameraController.SetPosition({-1.0f, 0.0f, 0.0f});
        m_CameraController.SetRotation(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));

        std::string path_to_project = CMAKE_PATH;
        m_ShaderLibrary.Load(path_to_project + "/assets/shaders/Grid.glsl");
        m_ShaderLibrary.Load(path_to_project + "/assets/shaders/Environment.glsl");


        // Screen quad with environment
        float vertices_cube[] = {
            // positions
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
        };
        //Vertex array
        m_CubeVA = VertexArray::Create();
        m_CubeVA->Bind();
        // Vertex buffer
        std::shared_ptr<VertexBuffer> cubeVB;
        cubeVB = VertexBuffer::Create(vertices_cube, sizeof(vertices_cube));
        cubeVB->Bind();
        cubeVB->SetLayout({
            {ShaderDataType::Float3, "a_Position"}
        });
        m_CubeVA->AddVertexBuffer(cubeVB);
        unsigned int indices_cube[] = {
            0, 1, 2, 3, 4, 5, 6,
            7, 8, 9, 10, 11, 12,
            13, 14, 15, 16, 17, 18,
            19, 20, 21, 22, 23, 24,
            25, 26, 27, 28, 29, 30,
            31, 32, 33, 34, 35,
        };
        std::shared_ptr<IndexBuffer> cubeIB;
        cubeIB = IndexBuffer::Create(indices_cube, sizeof(indices_cube) / sizeof(uint32_t));
        m_CubeVA->SetIndexBuffer(cubeIB);




        // Square with texture
        auto textureShader = m_ShaderLibrary.Load(path_to_project + "/assets/shaders/Texture.glsl");
        //auto textureShader = m_ShaderLibrary.Load("Texture.glsl");

        float vertices_square[5 * 4] = {
            0.0f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.0f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.0f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.0f, 0.5f, -0.5f, 0.0f, 1.0f,
        };

        //Vertex array
        m_SquareVA = VertexArray::Create();
        m_SquareVA->Bind();
        // Vertex buffer
        std::shared_ptr<VertexBuffer> squareVB;
        squareVB = VertexBuffer::Create(vertices_square, sizeof(vertices_square));
        squareVB->Bind();

        squareVB->SetLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float2, "a_TexCoord"}
        });
        m_SquareVA->AddVertexBuffer(squareVB);
        unsigned int indices_square[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<IndexBuffer> squareIB;
        squareIB = IndexBuffer::Create(indices_square, sizeof(indices_square) / sizeof(uint32_t));
        m_SquareVA->SetIndexBuffer(squareIB);

        RenderCommand::SetClearColor(glm::vec4(0., 0., 0., 1.0));

        // load texture
        m_Texture = Texture2D::Create(path_to_project + "/assets/textures/tex_coord.png");
        //m_Texture = Texture2D::Create("tex_coord.png");
        textureShader->Bind();
        textureShader->SetInt("u_Texture", 0);
        m_Texture->Bind(0);

        // Create scene
        // Create new entity with graphics component
        HE::Entity* entity = m_Scene->CreateEntity("null component");
        Component* graphicsComponent = new GraphicsComponent(entity);
        entity->AddComponent(ComponentType::GraphicsComponent, *graphicsComponent);

    }

    void SandBoxLayer::OnDetach()
    {

    }

    void SandBoxLayer::OnUpdate(Timestep& ts)
    {
        HE_PROFILE_FUNCTION();

        {
            HE_PROFILE_SCOPE("m_CameraController::OnUpdate");

            // Update
            m_CameraController.OnUpdate(ts);
        }

        {
            HE_PROFILE_SCOPE("Render prep");

            // Renderer
            RenderCommand::Clear();
        }

        {
            HE_PROFILE_SCOPE("Update Scene");
            m_Scene->OnUpdate(ts);
        }

        {
            HE_PROFILE_SCOPE("Renderer Draw");
            Renderer::BeginScene(m_CameraController.GetCamera());
            // environment

            auto environmentShader = m_ShaderLibrary.Get("Environment");
            environmentShader->Bind();
            environmentShader->SetMat4("u_ProjectionView", m_CameraController.GetCamera()->GetProjectionMatrix() * glm::mat4(glm::mat3(m_CameraController.GetCamera()->GetViewMatrix())));
            Renderer::Submit(environmentShader, m_CubeVA);

            // grid
            auto gridShader = m_ShaderLibrary.Get("Grid");
            gridShader->Bind();
            gridShader->SetMat4("u_ProjectionView", m_CameraController.GetCamera()->GetProjectionViewMatrix());
            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(0.0f, -0.5f, 0.0f));

            transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0, 0.0f));
            transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 0.0, 1.0f));
            transform = glm::scale(transform, glm::vec3(1.0f, 10.0f, 10.0f));

            gridShader->SetMat4("u_Model", transform);
            //gridShader->SetMat4("u_Model", glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0, 0.0f)));

            Renderer::Submit(gridShader, m_SquareVA);

            // quad with texture
            auto textureShader = m_ShaderLibrary.Get("Texture");
            textureShader->Bind();
            textureShader->SetMat4("u_ProjectionView", m_CameraController.GetCamera()->GetProjectionViewMatrix());
            textureShader->SetMat4("u_Model", glm::mat4(1.0f));
            m_Texture->Bind();
            Renderer::Submit(textureShader, m_SquareVA);

            Renderer::EndScene();
        }
    }

    void SandBoxLayer::OnImGuiRender()
    {
        HE_PROFILE_FUNCTION();



    }

    void SandBoxLayer::OnEvent(Event &e)
    {
        HE_PROFILE_FUNCTION();
        m_CameraController.OnEvent(e);
    }
}


