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

        //Renderer3D::Init();
        //Renderer2D::Init();
    }

    void Renderer::BeginRenderPass(std::shared_ptr<RenderPass> renderPass, bool clear)
    {
        HE_CORE_ASSERT(renderPass, "RenderPass cannot be nullptr!");

        // TODO: Convert all of this into a render command buffer
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
                    glEnable(GL_DEPTH_TEST);
                else
                    glDisable(GL_DEPTH_TEST);

                if (!material->GetFlag(MaterialFlag::TwoSided))
                    Renderer::Submit([]() { glEnable(GL_CULL_FACE); });
                else
                    Renderer::Submit([]() { glDisable(GL_CULL_FACE); });

                RenderCommand::DrawIndexed(mesh->m_VertexArray);
                //RenderCommand::DrawIndexed(submesh.IndexCount);
                //glDrawElementsBaseVertex(GL_TRIANGLES, submesh.IndexCount, GL_UNSIGNED_INT, (void*)(sizeof(uint32_t) * submesh.BaseIndex), submesh.BaseVertex);
                });
        }
    }

   

    void Renderer::Shutdown()
    {
        HE_PROFILE_FUNCTION();

        //Renderer3D::Shutdown();
        //Renderer2D::Shutdown();
    }
    /*
    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, uint32_t entityID, const glm::mat4& transform)
    {
        HE_PROFILE_FUNCTION();
        shader->Bind();
        shader->SetMat4("u_ProjectionView", s_Data->ProjectionView);
        shader->SetMat4("u_Model", transform);
        shader->SetInt("u_EntityID", entityID);
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);

    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform, std::shared_ptr<MaterialComponent> material)
    {
        HE_PROFILE_FUNCTION();
        shader->Bind();
        shader->SetMat4("u_ProjectionView", s_Data->ProjectionView);
        shader->SetMat4("u_Model", transform);
        if (material != nullptr)
        {
            auto textures = material->GetTextures();
            uint32_t i = 0;
            for (auto& [name, texture] : textures)
            {
                shader->SetInt(name, i);
                texture->Bind(i);
                i++;
            }
        }
        // Bind all lights uniforms
        glm::vec3 position(m_SceneData->View[3][0], m_SceneData->View[3][1], m_SceneData->View[3][2]);
        shader->SetVec3("u_ViewPosition", position);

        int NumDirectionalLights = 0;
        int NumPointLights = 0;
        int NumSpotLights = 0;
        for (auto& entity: m_SceneData->Lights)
        {
            LightComponent* lightComponent = entity->GetComponent<LightComponent>();
            TransformComponent* transformComponent =entity->GetComponent<TransformComponent>();
            if (lightComponent->GetLightType() == LightType::Directional)
            {
                shader->SetVec3("directionalLights[" + std::to_string(NumDirectionalLights) + "].l_Direction", lightComponent->GetDirection());
                shader->SetVec3("directionalLights[" + std::to_string(NumDirectionalLights) + "].l_Color", lightComponent->GetColor());
                shader->SetFloat("directionalLights[" + std::to_string(NumDirectionalLights) + "].l_Intensity", lightComponent->GetIntensity());
                shader->SetFloat("directionalLights[" + std::to_string(NumDirectionalLights) + "].l_Range", lightComponent->GetRange());
                NumDirectionalLights++;
            }
            else if (lightComponent->GetLightType() == LightType::Point)
            {
                shader->SetVec3("pointLights[" + std::to_string(NumPointLights) + "].l_Position", transformComponent->GetPosition());
                shader->SetVec3("pointLights[" + std::to_string(NumPointLights) + "].l_Color", lightComponent->GetColor());
                shader->SetFloat("pointLights[" + std::to_string(NumPointLights) + "].l_Intensity", lightComponent->GetIntensity());
                shader->SetFloat("pointLights[" + std::to_string(NumPointLights) + "].l_Range", lightComponent->GetRange());
                NumPointLights++;
            }
            else if (lightComponent->GetLightType() == LightType::Spot)
            {
                shader->SetVec3("spotLights[" + std::to_string(NumSpotLights) + "].l_Position", transformComponent->GetPosition());
                shader->SetVec3("spotLights[" + std::to_string(NumSpotLights) + "].l_Direction", lightComponent->GetDirection());
                shader->SetVec3("spotLights[" + std::to_string(NumSpotLights) + "].l_Color", lightComponent->GetColor());
                shader->SetFloat("spotLights[" + std::to_string(NumSpotLights) + "].l_Intensity", lightComponent->GetIntensity());
                shader->SetFloat("spotLights[" + std::to_string(NumSpotLights) + "].l_Range", lightComponent->GetRange());
                shader->SetFloat("spotLights[" + std::to_string(NumSpotLights) + "].l_InnerConeAngle", lightComponent->GetInnerConeAngle());
                shader->SetFloat("spotLights[" + std::to_string(NumSpotLights) + "].l_OuterConeAngle", lightComponent->GetOuterConeAngle());
                NumSpotLights++;
            }
        }
        shader->SetInt("u_NumDirectionalLights", NumDirectionalLights);
        shader->SetInt("u_NumPointLights", NumPointLights);
        shader->SetInt("u_NumSpotLights", NumSpotLights);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }


    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray> &vertexArray, const glm::mat4& transform)
    {
        HE_PROFILE_FUNCTION();

        //shader->Bind();
        // В будущем, при использовании системы материалов, mi - material instance (смотреть main.cpp):
        // mi->Bind()
        // этого больше не будет в Submit:

         // shader->Bind();
         // shader->SetMat4("u_ProjectionView", m_SceneData->ProjectionViewMatrix);
         // shader->SetMat4("u_Model", transform);
        

        //Должен добавлять в commandQUE
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
    */

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

