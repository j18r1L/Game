#ifndef OPENGLVERTEXARRAY_H
#define OPENGLVERTEXARRAY_H

#include "HartEng/pch.h"
#include "HartEng/Renderer/VertexArray.h"


namespace HE
{
    class OpenGLVertexArray: public VertexArray
    {
    private:
        uint32_t m_RendererID;

        std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
    public:
        OpenGLVertexArray(); 
        ~OpenGLVertexArray();

        void Bind() const override;
        void UnBind() const override;

        void AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer) override;
        void SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer) override;

        const std::vector<std::shared_ptr<VertexBuffer> > & GetVertexBuffers() const override
        {
            return m_VertexBuffers;
        }
        const std::shared_ptr<IndexBuffer> & GetIndexBuffer() const override
        {
            return m_IndexBuffer;
        }

    };
    
    
}

#endif // OPENGLVERTEXARRAY_H
