#include "HartEng/pch.h"
#include "HartEng/Renderer/Renderer.h"
#include "HartEng/Platform/OpenGL/OpenGLShader.h"



namespace HE
{
    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

    void Renderer::Init()
    {
        RenderCommand::Init();
    }

    void Renderer::BeginScene(std::shared_ptr<Camera> camera)
    {
        m_SceneData->ProjectionViewMatrix = camera->GetProjectionViewMatrix();
    }

    void Renderer::EndScene()
    {

    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray> &vertexArray, const glm::mat4& transform)
    {
        shader->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->SetMat4("u_ProjectionView", m_SceneData->ProjectionViewMatrix);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->SetMat4("u_Model", transform);
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
        // TODO когда будет много framebuffer-ов, то нужно будет резайзить нужные, а не только вьюшку
        RenderCommand::SetViewport(0, 0, width, height);
    }
}

