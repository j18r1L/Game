#ifndef OPENGLBUFFER_H
#define OPENGLBUFFER_H

#include "HartEng/pch.h"
#include "HartEng/Renderer/Buffer.h"


namespace HE
{
    class OpenGLVertexBuffer: public VertexBuffer
    {
    private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        virtual ~OpenGLVertexBuffer();
        void Bind() const override;
        void UnBind() const override;

        virtual void SetLayout(const BufferLayout& layout)
        {
            m_Layout = layout;
        }
        virtual const BufferLayout& GetLayout() const
        {
            return m_Layout;
        }

    };
    
    //////////////////////////////////////////////////////////// IndexBuffer ////////////////////////////////////////////////////////////////

    class OpenGLIndexBuffer: public IndexBuffer
    {
    private:
        uint32_t m_RendererID;
        uint32_t m_Count;
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
        virtual ~OpenGLIndexBuffer();
        void Bind() const override;
        void UnBind() const override;

        uint32_t GetCount() const override;
    };
}

#endif // OPENGLBUFFER_H
