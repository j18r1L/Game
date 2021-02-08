#include "HartEng/Platform/OpenGL/OpenGLBuffer.h"

#include "HartEng/Renderer/Renderer.h"
#include "HartEng/Core/pch.h"

#include <glad/glad.h>
namespace HE
{
    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
    {
        HE_PROFILE_FUNCTION();

        Renderer::Submit([this, vertices, size]() mutable
            {
                glGenBuffers(1, &this->m_RendererID);
                glBindBuffer(GL_ARRAY_BUFFER, this->m_RendererID);

                //TODO заменить захардкоженный STATIC DRAW
                glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
            });
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(void* vertices, uint32_t size)
    {
        HE_PROFILE_FUNCTION();

        Renderer::Submit([this, vertices, size]() mutable
            {
                glGenBuffers(1, &this->m_RendererID);
                glBindBuffer(GL_ARRAY_BUFFER, this->m_RendererID);

                //TODO заменить захардкоженный STATIC DRAW
                glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
            });
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        HE_PROFILE_FUNCTION();

        GLuint rendererID = m_RendererID;
        Renderer::Submit([rendererID]() mutable
            {
                glDeleteBuffers(1, &rendererID);
            });
    }

    void OpenGLVertexBuffer::Bind() const
    {
        HE_PROFILE_FUNCTION();

        Renderer::Submit([this]() 
            {
                glBindBuffer(GL_ARRAY_BUFFER, this->m_RendererID);
            });
    }

    void OpenGLVertexBuffer::UnBind() const
    {
        HE_PROFILE_FUNCTION();

        Renderer::Submit([]()
            {
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            });
    }

    //////////////////////////////////////////////////////////// IndexBuffer ////////////////////////////////////////////////////////////////

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count):
        m_Count(count)
    {
        HE_PROFILE_FUNCTION();

        Renderer::Submit([this, indices, count]() mutable
            {
                glGenBuffers(1, &this->m_RendererID);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_RendererID);

                //TODO заменить захардкоженный STATIC DRAW
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
            });
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        HE_PROFILE_FUNCTION();

        GLuint rendererID = m_RendererID;
        Renderer::Submit([rendererID]()
            {
                glDeleteBuffers(1, &rendererID);
            });
    }

    void OpenGLIndexBuffer::Bind() const
    {
        HE_PROFILE_FUNCTION();

        Renderer::Submit([this]()
            {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_RendererID);
            });
    }

    void OpenGLIndexBuffer::UnBind() const
    {
        HE_PROFILE_FUNCTION();

        Renderer::Submit([]()
            {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            });
    }

    uint32_t OpenGLIndexBuffer::GetCount() const
    {
        return m_Count;
    }

}

