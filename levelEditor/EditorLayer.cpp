#include "EditorLayer.h"


namespace HE
{
    EditorLayer::EditorLayer():
        Layer("EditorLayer"),
        m_CameraController(45.0f, Application::Get().GetWindow().GetWidth() / Application::Get().GetWindow().GetHeight(), 0.1f, 100.0f),
        m_ViewportSize(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight())
    {
        m_Scene = std::make_shared<Scene>(Scene("first_scene"));
        m_SceneHierarchyPanel = std::make_shared<SceneHierarchyPanel>(SceneHierarchyPanel());
        m_ShaderLibrary = std::make_shared<ShaderLibrary>(ShaderLibrary());

        RenderCommand::SetClearColor(glm::vec4(1.0, 0., 1.0, 1.0));
    }

    void EditorLayer::OnAttach()
    {
        HE_PROFILE_FUNCTION();

        std::string path_to_project = CMAKE_PATH;
        m_ShaderLibrary->Load(path_to_project + "/assets/shaders/Environment.glsl");
        /*
        m_ShaderLibrary->Load(path_to_project + "/assets/shaders/Grid.glsl");
        
        m_ShaderLibrary->Load(path_to_project + "/assets/shaders/Texture.glsl");

        
        // Create backpack entity
        Entity* backpackEntity = m_Scene->CreateEntity("Backpack");
        // create material, mesh, submesh components
        LoadMesh::CreateMesh(backpackEntity, path_to_project + "/assets/obj/backpack/backpack.obj");

        // Add shader to material
        auto backpackMaterialComponent = dynamic_cast<MaterialComponent*>(backpackEntity->GetComponent(ComponentType::MaterialComponent));
        backpackMaterialComponent->SetShader(m_ShaderLibrary, "Texture");


        // Create grid entity
        Entity* gridEntity = m_Scene->CreateEntity("Grid");
        LoadMesh::CreateMesh(gridEntity, path_to_project + "/assets/obj/quad/quad.obj");
        // Add shader to material
        auto gridMaterialComponent = dynamic_cast<MaterialComponent*>(gridEntity->GetComponent(ComponentType::MaterialComponent));
        gridMaterialComponent->SetShader(m_ShaderLibrary, "Grid");
        // Set transform component
        TransformComponent* gridTransformComponent = dynamic_cast<TransformComponent*>(gridEntity->GetComponent(ComponentType::TransformComponent));
        gridTransformComponent->SetPosition({10.0f, -0.5f, 10.0f});
        gridTransformComponent->SetRotation({0.0f, 90.0f, 0.0f});
        gridTransformComponent->SetRotation({0.0f, 0.0f, 90.0f});
        gridTransformComponent->SetScale({1.0f, 10.0f, 10.0f});
        */
        
        // Create environment entity
        environmentEntity = new Entity();
        environmentEntity->AddComponent(ComponentType::TransformComponent);
        if (LoadMesh::CreateMesh(environmentEntity, path_to_project + "/assets/obj/cube/cube.obj"))
        {
            // Add shader to material
            auto environmentMaterialComponent = dynamic_cast<MaterialComponent*>(environmentEntity->GetComponent(ComponentType::MaterialComponent));
            environmentMaterialComponent->SetShader(m_ShaderLibrary, "Environment");
        }
        
        

        // load framebuffer
        m_FrameBufferSpec.Width = Application::Get().GetWindow().GetWidth();
        m_FrameBufferSpec.Height = Application::Get().GetWindow().GetHeight();
        m_FrameBuffer = FrameBuffer::Create(m_FrameBufferSpec);

        // Create scene hirarchy panel
        m_SceneHierarchyPanel->SetScene(m_Scene);
        m_SceneHierarchyPanel->SetShaderLibrary(m_ShaderLibrary);
        
        //SceneSerializer serializer(m_Scene);
        //serializer.Deserialize(path_to_project + "/assets/scenes/example.he");




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
            if (m_ViewportFocused)
                m_CameraController.OnUpdate(ts);
        }

        {
            HE_PROFILE_SCOPE("Render prep");

            // Renderer
            m_FrameBuffer->Bind();
            RenderCommand::Clear();
        }
        {
            HE_PROFILE_SCOPE("Renderer Draw");
            
            // environment
            RenderCommand::SetDepthTest(false);
            if (environmentEntity->HasComponent(ComponentType::MeshComponent))
            {
                MeshComponent* meshComponent = dynamic_cast<MeshComponent*>(environmentEntity->GetComponent(ComponentType::MeshComponent));
                auto environmentShader = m_ShaderLibrary->Get("Environment");
                auto& subMeshes = meshComponent->GetSubMeshes();
                for (auto& subMesh : subMeshes)
                {
                    environmentShader->Bind();
                    environmentShader->SetMat4("u_ProjectionView", m_CameraController.GetCamera().GetProjection() * glm::mat4(glm::mat3(m_CameraController.GetCamera().GetView())));
                    auto& attribute = subMesh->GetAttribute();
                    Renderer::Submit(environmentShader, attribute);
                }
            }
            

            RenderCommand::SetDepthTest(true);
            //m_Scene->OnUpdate(ts); // Use runtime camera
            m_Scene->OnUpdate(ts, m_CameraController.GetCamera());
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

        static bool loadScene = false, saveScene = false; // SaveScene flag, LoadScene flag
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit"))
                    Application::Get().Close();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Scenes"))
            {
                if (ImGui::MenuItem("Save Scene"))
                {
                    saveScene = true;
                }
                if (ImGui::MenuItem("Load Scene"))
                {
                    loadScene = true;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        if (saveScene || loadScene)
        {
            SceneSerializer serializer(m_Scene, m_ShaderLibrary);
            std::string loadSceneWindowName = "";
            if (saveScene)
                loadSceneWindowName = "Save Scene";
            if (loadScene)
                loadSceneWindowName = "Load Scene";
            ImGui::Begin(loadSceneWindowName.c_str());
            static std::string FilePath(256, '\0');
            ImGui::InputText("Path", &FilePath[0], 256);


            if (ImGui::Button("Accept"))
            {
                if (saveScene)
                {
                    serializer.Serialize(FilePath);
                    saveScene = false;
                }
                    
                else if (loadScene)
                {
                    serializer.Deserialize(FilePath);
                    loadScene = false;
                }
                    
            }
            ImGui::End();
        }
        

        m_SceneHierarchyPanel->OnImGuiRender();


        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        ImGui::Begin("ViewPort");
        m_ViewportFocused = ImGui::IsWindowFocused();
        Application::Get().GetImGuiLayer()->SetBlockEvents(m_ViewportFocused);
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

        // Resize
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
                m_Scene->OnViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
                m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
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
        //EventDispatcher dispatcher(e);
        //m_CameraController.OnEvent(e);
    }

    void OnWindowResized()
    {

    }
}


