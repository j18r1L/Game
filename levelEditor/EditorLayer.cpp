#include "EditorLayer.h"


namespace HE
{
    EditorLayer::EditorLayer():
        Layer("EditorLayer"),
        m_CameraController(45.0f, Application::Get().GetWindow().GetWidth() / Application::Get().GetWindow().GetHeight(), 0.1f, 100.0f),
        m_ViewportSize(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight())
    {}

    void EditorLayer::OnAttach()
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
        textureShader->Bind();
        textureShader->SetInt("u_Texture", 0);
        m_Texture->Bind(0);

        // load framebuffer
        FrameBufferSpecification fbSpec;
        fbSpec.Width = Application::Get().GetWindow().GetWidth();
        fbSpec.Height = Application::Get().GetWindow().GetWidth();
        m_FrameBuffer = FrameBuffer::Create(fbSpec);
    }

    void EditorLayer::OnDetach()
    {

    }

    void EditorLayer::OnUpdate(Timestep& ts)
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
            HE_PROFILE_SCOPE("Renderer Draw");
            m_FrameBuffer->Bind();
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
            m_FrameBuffer->UnBind();
        }
    }

    void EditorLayer::OnImGuiRender()
    {
        HE_PROFILE_FUNCTION();

        static bool dockSpaceOpen = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->GetWorkPos());
            ImGui::SetNextWindowSize(viewport->GetWorkSize());
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }


        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit"))
                    Application::Get().Close();
                ImGui::EndMenu();
            }


            ImGui::EndMenuBar();
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        ImGui::Begin("ViewPort");
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        if (m_ViewportSize != glm::vec2(viewportPanelSize.x, viewportPanelSize.y))
        {
            if (m_ViewportSize.x <= 1.0f || m_ViewportSize.y <= 1.0f)
            {
                HE_CORE_ERROR("Viewport width or height is below zero!");
            }
            else
            {
                m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
                m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};
            }

        }


        uint32_t FrameBufferID = m_FrameBuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*) FrameBufferID, ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0.0f, 1.0f}, ImVec2{1.0f, 0.0f});

        ImGui::PopStyleVar();

        // Viewport end
        ImGui::End();


        // Docking Space end
        ImGui::End();



    }

    void EditorLayer::OnEvent(Event &e)
    {
        HE_PROFILE_FUNCTION();
        EventDispatcher dispatcher(e);
        m_CameraController.OnEvent(e);
    }

    void OnWindowResized()
    {

    }
}


