#include "HartEng/Renderer/Renderer.h"

namespace HE
{
    void Renderer::BeginScene()
    {

    }

    void Renderer::EndScene()
    {

    }

    void Renderer::Submit(const std::shared_ptr<VertexArray> &vertexArray)
    {
        //Должен добавлять в commandQUE
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}

