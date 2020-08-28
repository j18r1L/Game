#include "HartEng/Core/pch.h"
#include "HartEng/Renderer/Renderer.h"
#include "HartEng/Platform/OpenGL/OpenGLShader.h"



namespace HE
{
    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;



    void Renderer::BeginScene(std::shared_ptr<Camera> camera)
    {
        HE_PROFILE_FUNCTION();

        m_SceneData->ProjectionViewMatrix = camera->GetProjectionViewMatrix();
        m_SceneData->ViewMatrix = camera->GetViewMatrix();
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

