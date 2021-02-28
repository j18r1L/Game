#include "HartEng/Core/pch.h"
#include "HartEng/Renderer/Renderer.h"
#include "HartEng/Renderer/SceneRenderer.h"
#include "HartEng/Platform/OpenGL/OpenGLShader.h"
#include "HartEng/Scene/Components/LightComponent.h"
#include "HartEng/Scene/Components/TransformComponent.h"


namespace HE
{
    struct RendererData
    {
        std::shared_ptr<RenderPass> m_ActiveRenderPass;
        RenderCommandQueue m_CommandQueue;
        std::shared_ptr<ShaderLibrary> m_ShaderLibrary;
    };

    static RendererData s_RenderData;

    void Renderer::Init()
    {
        HE_PROFILE_FUNCTION();

        RenderCommand::Init();
        s_RenderData.m_ShaderLibrary = std::shared_ptr<ShaderLibrary>(new ShaderLibrary());

        SceneRenderer::Init();

        // Now we can load shaders that 100% will be used
        std::string PathToProject = CMAKE_PATH;
        Renderer::GetShaderLibrary()->Load(PathToProject + "/assets/shaders/Black.glsl");
        Renderer::GetShaderLibrary()->Load(PathToProject + "/assets/shaders/EntityID.glsl");
        //Renderer3D::Init();
        //Renderer2D::Init();
    }

    void Renderer::BeginRenderPass(std::shared_ptr<RenderPass> renderPass, bool clear)
    {
        HE_CORE_ASSERT(renderPass, "RenderPass cannot be nullptr!");

        s_RenderData.m_ActiveRenderPass = renderPass;

        renderPass->GetSpecification().TargetFramebuffer->Bind();
        if (clear)
        {
            const glm::vec4& clearColor = renderPass->GetSpecification().TargetFramebuffer->GetSpecification().ClearColor;
            Renderer::Submit([clearColor]()
                {
                    RenderCommand::SetClearColor(clearColor);
                    RenderCommand::Clear();
                });
        }
    }

    void Renderer::EndRenderPass()
    {
        HE_CORE_ASSERT(s_RenderData.m_ActiveRenderPass, "No active renderPass! Call BeginRenderPass first, or you called EndRenderPass twice!");
        s_RenderData.m_ActiveRenderPass->GetSpecification().TargetFramebuffer->UnBind();
        s_RenderData.m_ActiveRenderPass = nullptr;
    }

    void Renderer::WaitAndRender()
    {
        s_RenderData.m_CommandQueue.Execute();
    }

    void Renderer::SubmitMesh(std::shared_ptr<Mesh> mesh, const glm::mat4& transform, std::shared_ptr<MaterialInstance> overrideMaterial)
    {
        // TODO: Sort this out
        mesh->m_VertexBuffer->Bind();
        mesh->m_VertexArray->Bind();
        mesh->m_IndexBuffer->Bind();

        auto& materials = mesh->GetMaterials();
        for (Submesh& submesh : mesh->m_Submeshes)
        {
            // Material
            auto material = overrideMaterial ? overrideMaterial : materials[submesh.MaterialIndex];
            auto shader = material->GetShader();
            material->Bind();

            shader->SetMat4("u_Transform", transform * submesh.Transform);

            Renderer::Submit([mesh, submesh, material]() {
                if (material->GetFlag(MaterialFlag::DepthTest))
                    RenderCommand::SetDepthTest(true);
                else
                    RenderCommand::SetDepthTest(false);

                if (!material->GetFlag(MaterialFlag::TwoSided))
                    glEnable(GL_CULL_FACE);
                else
                    glDisable(GL_CULL_FACE);

                if (material->GetFlag(MaterialFlag::LineMode))
                {
                    glEnable(GL_LINE_SMOOTH);
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
                else
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

                glDrawElementsBaseVertex(GL_TRIANGLES, submesh.IndexCount, GL_UNSIGNED_INT, (void*)(sizeof(uint32_t) * submesh.BaseIndex), submesh.BaseVertex);
                });
        }
    }

   

    void Renderer::Shutdown()
    {
        HE_PROFILE_FUNCTION();

        //Renderer3D::Shutdown();
        //Renderer2D::Shutdown();
    }
    
    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        HE_PROFILE_FUNCTION();

        // TODO когда будет много framebuffer-ов, то нужно будет резайзить нужные, а не только вьюшку
        RenderCommand::SetViewport(0, 0, width, height);
    }

    std::shared_ptr<ShaderLibrary> Renderer::GetShaderLibrary()
    {
        return s_RenderData.m_ShaderLibrary;
    }

    RenderCommandQueue& Renderer::GetRenderCommandQueue()
    {
        return s_RenderData.m_CommandQueue;
    }
}

