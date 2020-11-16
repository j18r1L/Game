#include "SandBoxLayer.h"


namespace HE
{
    SandBoxLayer::SandBoxLayer():
        Layer("SandBoxLayer")
    {
        m_Scene = std::make_shared<Scene>(Scene("first_scene"));
        RenderCommand::SetClearColor(glm::vec4(0.5, 0., 0.5, 1.0));


    }

    void SandBoxLayer::OnAttach()
    {
        HE_PROFILE_FUNCTION();

        std::string path_to_project = CMAKE_PATH;
        m_ShaderLibrary.Load(path_to_project + "/assets/shaders/Grid.glsl");
        m_ShaderLibrary.Load(path_to_project + "/assets/shaders/Environment.glsl");
        m_ShaderLibrary.Load(path_to_project + "/assets/shaders/Texture.glsl");


        // Create backpack entity
        Entity* backpackEntity = m_Scene->CreateEntity("Backpack");
        // create material, mesh, submesh components
        LoadMesh::CreateMesh(backpackEntity, path_to_project + "/assets/obj/backpack/untitled.obj");

        // Add shader to material
        auto backpackMaterialComponent = dynamic_cast<MaterialComponent*>(backpackEntity->GetComponent(ComponentType::MaterialComponent));
        backpackMaterialComponent->SetShader(std::make_shared<ShaderLibrary>(m_ShaderLibrary), "Texture");


        // Create grid entity
        Entity* gridEntity = m_Scene->CreateEntity("Grid");
        LoadMesh::CreateMesh(gridEntity, path_to_project + "/assets/obj/quad/quad.obj");
        // Add shader to material
        auto gridMaterialComponent = dynamic_cast<MaterialComponent*>(gridEntity->GetComponent(ComponentType::MaterialComponent));
        gridMaterialComponent->SetShader(std::make_shared<ShaderLibrary>(m_ShaderLibrary), "Grid");
        // Set transform component
        TransformComponent* gridTransformComponent = dynamic_cast<TransformComponent*>(gridEntity->GetComponent(ComponentType::TransformComponent));
        gridTransformComponent->SetPosition({10.0f, -0.5f, 10.0f});
        gridTransformComponent->SetRotation({0.0f, 90.0f, 0.0f});
        gridTransformComponent->SetRotation({0.0f, 0.0f, 90.0f});
        gridTransformComponent->SetScale({1.0f, 10.0f, 10.0f});


        // Create environment entity
        environmentEntity = new Entity();
        environmentEntity->AddComponent(ComponentType::TransformComponent);
        LoadMesh::CreateMesh(environmentEntity, path_to_project + "/assets/obj/cube/cube.obj");
        // Add shader to material
        auto environmentMaterialComponent = dynamic_cast<MaterialComponent*>(environmentEntity->GetComponent(ComponentType::MaterialComponent));
        environmentMaterialComponent->SetShader(std::make_shared<ShaderLibrary>(m_ShaderLibrary), "Environment");


        // --------- Create Camera entity ------------------
        Entity* cameraEntity = m_Scene->CreateEntity("Camera");

        // Create camera component
        float aspectRatio = static_cast<float>(Application::Get().GetWindow().GetWidth()) / static_cast<float>(Application::Get().GetWindow().GetHeight());
        CameraComponent* cameraComponent = dynamic_cast<CameraComponent*>(cameraEntity->AddComponent(ComponentType::CameraComponent));
        cameraComponent->SetPrimary(true);
        cameraComponent->SetPerspective(45.0f, aspectRatio, 0.1f, 1000.0f);

    }

    void SandBoxLayer::OnDetach()
    {

    }

    void SandBoxLayer::OnUpdate(Timestep& ts)
    {
        HE_PROFILE_FUNCTION();


        {
            HE_PROFILE_SCOPE("Render prep");

            // Renderer
            RenderCommand::Clear();
        }

        {
            HE_PROFILE_SCOPE("Renderer Draw");

            // environment
            RenderCommand::SetDepthTest(false);
            MeshComponent* meshComponent = dynamic_cast<MeshComponent*>(environmentEntity->GetComponent(ComponentType::MeshComponent));
            glm::mat4 transform = dynamic_cast<TransformComponent*>(environmentEntity->GetComponent(ComponentType::TransformComponent))->GetTransform();
            glm::mat4 projection = dynamic_cast<CameraComponent*>(m_Scene->GetEntity("Camera")->GetComponent(ComponentType::CameraComponent))->GetCamera().GetProjection();
            auto environmentShader = m_ShaderLibrary.Get("Environment");
            auto& subMeshes = meshComponent->GetSubMeshes();
            environmentShader->Bind();

            environmentShader->SetMat4("u_ProjectionView", projection * glm::mat4(glm::mat3(glm::inverse(transform))));
            auto& attribute = subMeshes[0]->GetAttribute();
            Renderer::Submit(environmentShader, attribute);

            RenderCommand::SetDepthTest(true);
            m_Scene->OnUpdate(ts);
        }
    }

    void SandBoxLayer::OnImGuiRender()
    {
        HE_PROFILE_FUNCTION();



    }

    void SandBoxLayer::OnEvent(Event &e)
    {
        HE_PROFILE_FUNCTION();
    }
}


