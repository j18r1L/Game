#include "TestLayer.h"

TestLayer::TestLayer():
    Layer("TestLayer"),
    m_CameraController(45.0f, HE::Application::Get().GetWindow().GetWidth() / HE::Application::Get().GetWindow().GetHeight(), 0.1f, 100.0f)
{}

void TestLayer::OnAttach()
{
    HE_PROFILE_FUNCTION();

    m_CameraController.SetPosition({-1.0f, 0.0f, 0.0f});
    m_CameraController.SetRotation(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    m_ShaderLibrary.Load("../assets/shaders/Grid.glsl");
    m_ShaderLibrary.Load("../assets/shaders/Environment.glsl");
    //m_ShaderLibrary.Load("Grid.glsl");
    //m_ShaderLibrary.Load("Environment.glsl");

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
    m_CubeVA = HE::VertexArray::Create();
    m_CubeVA->Bind();
    // Vertex buffer
    std::shared_ptr<HE::VertexBuffer> cubeVB;
    cubeVB = HE::VertexBuffer::Create(vertices_cube, sizeof(vertices_cube));
    cubeVB->Bind();
    cubeVB->SetLayout({
        {HE::ShaderDataType::Float3, "a_Position"}
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
    std::shared_ptr<HE::IndexBuffer> cubeIB;
    cubeIB = HE::IndexBuffer::Create(indices_cube, sizeof(indices_cube) / sizeof(uint32_t));
    m_CubeVA->SetIndexBuffer(cubeIB);




    // Square with texture
    auto textureShader = m_ShaderLibrary.Load("../assets/shaders/Texture.glsl");
    //auto textureShader = m_ShaderLibrary.Load("Texture.glsl");

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
    //m_Texture = HE::Texture2D::Create("tex_coord.png");
    textureShader->Bind();
    textureShader->SetInt("u_Texture", 0);
    m_Texture->Bind(0);
}

void TestLayer::OnDetach()
{

}

void TestLayer::OnUpdate(HE::Timestep& ts)
{
    HE_PROFILE_FUNCTION();

    {
        HE_PROFILE_SCOPE("m_CameraController::OnUpdate");

        // Update
        m_CameraController.OnUpdate(ts);
    }

    {
        HE_PROFILE_SCOPE("RenderCommand::Clear");

        // Renderer
        HE::RenderCommand::Clear();
    }
    {
        HE_PROFILE_SCOPE("Renderer Draw");

        HE::Renderer::BeginScene(m_CameraController.GetCamera());
        // environment
        
        auto environmentShader = m_ShaderLibrary.Get("Environment");
        environmentShader->Bind();
        environmentShader->SetMat4("u_ProjectionView", m_CameraController.GetCamera()->GetProjectionMatrix() * glm::mat4(glm::mat3(m_CameraController.GetCamera()->GetViewMatrix())));
        HE::Renderer::Submit(environmentShader, m_CubeVA);

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
        HE::Renderer::Submit(gridShader, m_SquareVA);

        // quad with texture
        auto textureShader = m_ShaderLibrary.Get("Texture");
        textureShader->Bind();
        textureShader->SetMat4("u_ProjectionView", m_CameraController.GetCamera()->GetProjectionViewMatrix());
        textureShader->SetMat4("u_Model", glm::mat4(1.0f));
        HE::Renderer::Submit(textureShader, m_SquareVA);
        
        HE::Renderer::EndScene();
    }
}

void TestLayer::OnImGuiRender()
{
    HE_PROFILE_FUNCTION();

}

void TestLayer::OnEvent(HE::Event &e)
{
    HE_PROFILE_FUNCTION();
    m_CameraController.OnEvent(e);
}

