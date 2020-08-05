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
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        virtual ~OpenGLVertexBuffer();
        void Bind() const override;
        void UnBind() const override;
    };
    
    //////////////////////////////////////////////////////////// IndexBuffer ////////////////////////////////////////////////////////////////

    class OpenGLIndexBuffer: public IndexBuffer
    {
    private:
        uint32_t m_RendererID;
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
        virtual ~OpenGLIndexBuffer();
        void Bind() const override;
        void UnBind() const override;
    };
}

#endif // OPENGLBUFFER_H
