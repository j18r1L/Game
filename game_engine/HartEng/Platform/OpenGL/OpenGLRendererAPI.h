#ifndef OPENGLRENDERERAPI_H
#define OPENGLRENDERERAPI_H

#include "HartEng/Renderer/RendererAPI.h"

namespace HE
{
    class OpenGLRendererAPI: public RendererAPI
    {
    private:

    public:
        void Init() override;
        void Clear() override;
        void SetClearColor(const glm::vec4 &color) override;

        void DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray) override;

    };
}

#endif // OPENGLRENDERERAPI_H
