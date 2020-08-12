#include "HartEng/Renderer/Renderer.h"
#include "HartEng/pch.h"

namespace HE
{
    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        m_SceneData->ProjectionViewMatrix = camera.GetProjectionViewMatrix();
    }

    void Renderer::EndScene()
    {

    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray> &vertexArray, const glm::mat4& transform)
    {
        shader->Bind();
        shader->SetMat4("u_ProjectionView", m_SceneData->ProjectionViewMatrix);
        shader->SetMat4("u_Model", transform);
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
}

