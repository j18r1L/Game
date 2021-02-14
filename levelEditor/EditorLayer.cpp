#include "EditorLayer.h"


#include "HartEng/Core/Utils.h"


namespace HE
{
    EditorLayer::EditorLayer() :
        Layer("EditorLayer"),
        m_CameraController(45.0f, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight(), 0.1f, 100.0f),
        m_ViewportSize(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight())
    {
        m_Gizmo.SetCamera(&m_CameraController);
        m_Scene = std::make_shared<Scene>(Scene("game_scene"));
        m_SceneHierarchyPanel = std::make_shared<SceneHierarchyPanel>(SceneHierarchyPanel());

        RenderCommand::SetClearColor(glm::vec4(1.0, 0., 1.0, 1.0));
    }

    void EditorLayer::OnAttach()
    {
        HE_PROFILE_FUNCTION();

        std::string path_to_project = CMAKE_PATH;

        SceneSerializer serializer(m_Scene, Renderer::GetShaderLibrary());
        serializer.Deserialize(path_to_project + "/assets/scenes/game.he");

        {
            Entity* entity = m_Scene->GetEntity("Ship");
            auto script = new PlayerControllerScript(entity);
            entity->AddComponent<ScriptComponent>(script);
            
            
        }

        {
            Entity* entity = m_Scene->GetEntity("Obstacle_1");
            auto script = new ObstacleScript(entity);
            entity->AddComponent<ScriptComponent>(script);
            //auto collider = entity->AddComponent<Circle2DColliderComponent>();
            //collider->SetRadius(1.0f);
            //collider->SetOrigin({ entity->GetComponent<TransformComponent>()->GetPosition().x, entity->GetComponent<TransformComponent>()->GetPosition().y });
        }
        /*
        Entity* obstacle = m_Scene->CreateEntity("Obstacle_1");
        obstacle->AddComponent<MeshComponent>()->SetMesh(std::make_shared<Mesh>(path_to_project + "/assets/meshes/obj/quad.obj"));
        ObstacleScript* obstacleScript = new ObstacleScript(obstacle);
        obstacle->AddComponent<ScriptComponent>(obstacleScript);
        */
        // Create scene hirarchy panel
        m_SceneHierarchyPanel->SetScene(m_Scene);
        m_SceneHierarchyPanel->SetShaderLibrary(Renderer::GetShaderLibrary());
    }

    void EditorLayer::OnScenePlay()
    {
        m_SceneState = SceneState::Play;
    }

    void EditorLayer::OnSceneStop()
    {
        m_SceneState = SceneState::Edit;
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
            if (m_ViewportFocused && m_SceneState != SceneState::Play)
                m_CameraController.OnUpdate(ts);
        }

        {
            HE_PROFILE_SCOPE("Render prep");
        }
        {
            HE_PROFILE_SCOPE("Renderer Draw");
            Draw(ts);
        }

    }

    void EditorLayer::Draw(Timestep& ts)
    {
        // Draw scene
        if (m_SceneState == SceneState::Play)
        {
            m_Scene->OnUpdate(ts); // Use runtime camera
            m_Scene->OnRenderRuntime(ts);
        }
        else
        {
            m_Scene->OnRenderEditor(ts, m_CameraController.GetCamera());
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

                if (ImGui::MenuItem("New Scene"))
                    if (m_Scene)
                        m_Scene->Clear();

                if (ImGui::MenuItem("Open Scene"))
                    loadScene = true;

                if (ImGui::MenuItem("Save Scene"))
                    saveScene = true;

                if (ImGui::MenuItem("Exit"))
                    Application::Get().Close();


                ImGui::EndMenu();

            }
            if (ImGui::BeginMenu("Scene"))
            {
                if (ImGui::MenuItem("Play"))
                {
                    m_SceneState = SceneState::Play;
                    m_Scene->OnRuntimeStart();
                }
                if (ImGui::MenuItem("Pause"))
                {
                    m_SceneState = SceneState::Pause;
                }  
                if (ImGui::MenuItem("Stop"))
                {
                    m_SceneState = SceneState::Edit;
                    m_Scene->OnRuntimeStop();
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        if (saveScene || loadScene)
        {
            SceneSerializer serializer(m_Scene, Renderer::GetShaderLibrary());
#ifdef HE_PLATFORM_WINDOWS
            if (loadScene)
            {
                std::string filepath = FileDialog::OpenFile("HartEng Scene (*.he)\0*.he\0 ");
                if (!filepath.empty())
                {
                    serializer.Deserialize(filepath);
                    m_Scene->OnViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
                    m_SceneHierarchyPanel->SetScene(m_Scene);
                    loadScene = false;
                }
            }
            if (saveScene)
            {
                std::string filepath = FileDialog::SaveFile("HartEng Scene (*.he)\0*.he\0 ");
                if (!filepath.empty())
                {
                    serializer.Serialize(filepath);
                    saveScene = false;
                }
            }


#elif HE_PLATFORM_LINUX
            std::string loadSceneWindowName = "";
            if (saveScene)
                loadSceneWindowName = "Save Scene";
            if (loadScene)
                loadSceneWindowName = "Open Scene";
            ImGui::Begin(loadSceneWindowName.c_str());
            static std::string filepath(256, '\0');
            ImGui::InputText("Path", &filepath[0], 256);


            if (ImGui::Button("Accept"))
            {
                if (saveScene)
                {
                    serializer.Serialize(filepath);
                    saveScene = false;
                }

                else if (loadScene)
                {
                    serializer.Deserialize(filepath);
                    loadScene = false;
                }

            }
            ImGui::End();
#endif

        }


        m_SceneHierarchyPanel->OnImGuiRender();


        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        bool viewportOpen = true;
        ImGuiWindowFlags ViewportWindow_flags = ImGuiWindowFlags_NoTitleBar;
        ImGui::Begin("ViewPort", &viewportOpen, ViewportWindow_flags);
        auto viewportOffset = ImGui::GetCursorPos();

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
                m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
                SceneRenderer::SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
                m_Scene->OnViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
                m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
            }
        }


        uint32_t FrameBufferID = SceneRenderer::GetGeometryRenderPass().get()->GetSpecification().TargetFramebuffer.get()->GetColorAttachmentRendererID();
        ImGui::Image((void*)FrameBufferID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });

        auto windowSize = ImGui::GetWindowSize();
        ImVec2 minBound = ImGui::GetWindowPos();
        minBound.x += viewportOffset.x;
        minBound.y += viewportOffset.y;

        ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
        m_ViewportBounds[0] = { minBound.x, minBound.y };
        m_ViewportBounds[1] = { maxBound.x, maxBound.y };

        // Draw Gizmo
        Entity* selectedEntity = m_SceneHierarchyPanel->GetSelectedEntity();
        if (selectedEntity && m_SceneState != SceneState::Play)
        {
            m_Gizmo.Draw(selectedEntity);
        }

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
        dispatcher.Dispatch<MouseButtonPressedEvent>(HE_BIND_EVENT_FN(EditorLayer::OnMouseButton));
        
    }

    bool EditorLayer::OnMouseButton(MouseButtonPressedEvent& event)
    {
        if (event.GetMouseButton() == HE_MOUSE_BUTTON_1 && !m_Gizmo.IsUsing() && !m_Gizmo.IsOver() && m_SceneState != SceneState::Play)
        {
            auto EntityIDFramebuffer = SceneRenderer::GetEntityIDRenderPass().get()->GetSpecification().TargetFramebuffer.get();
            EntityIDFramebuffer->Bind();

            auto [mx_a, my_a] = ImGui::GetMousePos();
            float mx = mx_a, my = my_a;
            mx -= m_ViewportBounds[0].x;
            my -= m_ViewportBounds[0].y;
            auto viewportWidth = m_ViewportBounds[1].x - m_ViewportBounds[0].x;
            auto viewportHeight = m_ViewportBounds[1].y - m_ViewportBounds[0].y;
            my = viewportHeight - my;
            if (mx >= 0.0f && my >= 0.0f && mx < viewportWidth && my < viewportHeight)
            {
                HE_CORE_TRACE("mx, my: {0}, {1}", mx, my);

                int entityID;
                Renderer::Submit([EntityIDFramebuffer, &entityID, mx, my]() mutable
                    {
                        entityID = EntityIDFramebuffer->ReadPixel(0, mx, my);
                    });
                HE_CORE_TRACE("EntityID: {0}", entityID);
                Entity* selectedEntity = m_Scene->GetEntity(entityID);
                m_SceneHierarchyPanel->SetSelectedEntity(selectedEntity);
            }
            EntityIDFramebuffer->UnBind();
        }
        return false;
    }


}


