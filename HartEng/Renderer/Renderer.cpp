#include "HartEng/Core/pch.h"
#include "HartEng/Renderer/Renderer.h"
#include "HartEng/Platform/OpenGL/OpenGLShader.h"
#include "HartEng/Scene/Components/LightComponent.h"
#include "HartEng/Scene/Components/TransformComponent.h"


namespace HE
{
    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;



    void Renderer::BeginScene(PerspectiveCamera& camera, const std::vector<Entity*>& lights)
    {
        HE_PROFILE_FUNCTION();

        m_SceneData->ProjectionView = camera.GetProjectionView();
        m_SceneData->View = camera.GetView();
        m_SceneData->Lights = lights;
    }

    void Renderer::BeginScene(const glm::mat4& projection, const glm::mat4 transform, const std::vector<Entity*>& lights)
    {
        HE_PROFILE_FUNCTION();

        m_SceneData->View = glm::inverse(transform);
        m_SceneData->ProjectionView = projection * m_SceneData->View;
        m_SceneData->Lights = lights;

    }


    void Renderer::EndScene()
    {
        HE_PROFILE_FUNCTION();


    }

    void Renderer::Init()
    {
        HE_PROFILE_FUNCTION();

        RenderCommand::Init();

        //Renderer3D::Init();
        //Renderer2D::Init();
    }

    void Renderer::Shutdown()
    {
        HE_PROFILE_FUNCTION();

        //Renderer3D::Shutdown();
        //Renderer2D::Shutdown();
    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, uint32_t entityID, const glm::mat4& transform)
    {
        HE_PROFILE_FUNCTION();
        shader->Bind();
        shader->SetMat4("u_ProjectionView", m_SceneData->ProjectionView);
        shader->SetMat4("u_Model", transform);
        shader->SetInt("u_EntityID", entityID);
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);

    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform, std::shared_ptr<MaterialComponent> material)
    {
        HE_PROFILE_FUNCTION();
        shader->Bind();
        shader->SetMat4("u_ProjectionView", m_SceneData->ProjectionView);
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
        /*
         * shader->Bind();
         * shader->SetMat4("u_ProjectionView", m_SceneData->ProjectionViewMatrix);
         * shader->SetMat4("u_Model", transform);
        */

        //Должен добавлять в commandQUE
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        HE_PROFILE_FUNCTION();

        // TODO когда будет много framebuffer-ов, то нужно будет резайзить нужные, а не только вьюшку
        RenderCommand::SetViewport(0, 0, width, height);
    }
}

