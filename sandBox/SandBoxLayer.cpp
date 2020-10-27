#include "SandBoxLayer.h"


namespace HE
{
    SandBoxLayer::SandBoxLayer():
        Layer("SandBoxLayer"),
        m_CameraController(45.0f, Application::Get().GetWindow().GetWidth() / Application::Get().GetWindow().GetHeight(), 0.1f, 100.0f)
    {
        m_Scene = std::make_unique<Scene>(Scene("first_scene"));
        RenderCommand::SetClearColor(glm::vec4(0.5, 0., 0.5, 1.0));

        m_CameraController.SetPosition({-1.0f, 0.0f, 0.0f});
        m_CameraController.SetRotation(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));


    }

    void SandBoxLayer::OnAttach()
    {
        HE_PROFILE_FUNCTION();

        std::string path_to_project = CMAKE_PATH;
        m_ShaderLibrary.Load(path_to_project + "/assets/shaders/Grid.glsl");
        m_ShaderLibrary.Load(path_to_project + "/assets/shaders/Environment.glsl");
        m_ShaderLibrary.Load(path_to_project + "/assets/shaders/Texture.glsl");


        // cube
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




        // square
        float vertices_square[5 * 4] = {
            0.0f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.0f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.0f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.0f, 0.5f, -0.5f, 0.0f, 1.0f,
        };

        //Vertex array
        std::shared_ptr<VertexArray> SquareVA;
        SquareVA = VertexArray::Create();
        SquareVA->Bind();
        // Vertex buffer
        std::shared_ptr<VertexBuffer> squareVB;
        squareVB = VertexBuffer::Create(vertices_square, sizeof(vertices_square));
        squareVB->Bind();

        squareVB->SetLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float2, "a_TexCoord"}
        });
        SquareVA->AddVertexBuffer(squareVB);
        unsigned int indices_square[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<IndexBuffer> squareIB;
        squareIB = IndexBuffer::Create(indices_square, sizeof(indices_square) / sizeof(uint32_t));
        SquareVA->SetIndexBuffer(squareIB);

        // Create grid entity
        Entity* gridEntity = m_Scene->CreateEntity("grid");

        // Set transform component
        TransformComponent* gridTransformComponent = dynamic_cast<TransformComponent*>(gridEntity->GetComponent(ComponentType::TransformComponent));
        gridTransformComponent->SetTranslation({0.0f, -0.5f, 0.0f});
        gridTransformComponent->SetRotation({0.0f, 90.0f, 0.0f});
        gridTransformComponent->SetRotation({0.0f, 0.0f, 90.0f});
        gridTransformComponent->SetScale({1.0f, 10.0f, 10.0f});

        // Create SubMesh
        SubMeshComponent* gridSubMeshComponent = new SubMeshComponent(gridEntity);
        gridSubMeshComponent->SetShader(std::make_shared<ShaderLibrary>(m_ShaderLibrary), "Grid");
        gridSubMeshComponent->SetAttribute("quad", SquareVA);
        gridEntity->AddComponent(ComponentType::SubMeshComponent, *gridSubMeshComponent);

        // Create mesh
        MeshComponent* gridMeshComponent = new MeshComponent(gridEntity);
        gridMeshComponent->AddSubMesh(*gridSubMeshComponent);
        gridEntity->AddComponent(ComponentType::MeshComponent, *gridMeshComponent);



        // Create square entity
        Entity* squareEntity = m_Scene->CreateEntity("square");

        // load and create texture
        Texture2DComponent* squareTexture2DComponent = new Texture2DComponent(squareEntity, path_to_project + "/assets/textures/tex_coord.png");
        squareEntity->AddComponent(ComponentType::Texture2DComponent, *squareTexture2DComponent);

        // Create material with texture
        MaterialComponent* squareMaterialComponent = new MaterialComponent(squareEntity);
        squareMaterialComponent->AddTexture("u_Texture", squareTexture2DComponent);
        squareEntity->AddComponent(ComponentType::MaterialComponent, *squareMaterialComponent);

        // Create SubMesh
        SubMeshComponent* squareSubMeshComponent = new SubMeshComponent(squareEntity);
        squareSubMeshComponent->SetMaterial(*squareMaterialComponent);
        squareSubMeshComponent->SetShader(std::make_shared<ShaderLibrary>(m_ShaderLibrary), "Texture");
        squareSubMeshComponent->SetAttribute("quad", SquareVA);
        squareEntity->AddComponent(ComponentType::SubMeshComponent, *squareSubMeshComponent);

        // Create Mesh
        MeshComponent* squareMeshComponent = new MeshComponent(squareEntity);
        squareMeshComponent->AddSubMesh(*squareSubMeshComponent);
        squareEntity->AddComponent(ComponentType::MeshComponent, *squareMeshComponent);


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

        /*
        {
            HE_PROFILE_SCOPE("Update Scene");
            m_Scene->OnUpdate(ts);
        }
        */
        {
            HE_PROFILE_SCOPE("Renderer Draw");
            Renderer::BeginScene(m_CameraController.GetCamera());

            RenderCommand::SetDepthTest(true);
            m_Scene->OnUpdate(ts);

            /*
            RenderCommand::SetDepthTest(false);
            // environment
            auto environmentShader = m_ShaderLibrary.Get("Environment");
            environmentShader->Bind();
            environmentShader->SetMat4("u_ProjectionView", m_CameraController.GetCamera()->GetProjectionMatrix() * glm::mat4(glm::mat3(m_CameraController.GetCamera()->GetViewMatrix())));
            Renderer::Submit(environmentShader, m_CubeVA);
            */

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


