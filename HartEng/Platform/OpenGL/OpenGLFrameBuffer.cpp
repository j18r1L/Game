#include "OpenGLFrameBuffer.h"
#include "HartEng/Core/pch.h"

#include "HartEng/Core/Core.h"
#include "HartEng/Core/Log.h"

#include <glad/glad.h>

namespace HE
{
    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec):
        m_Specification(spec)
    {
        Invalidate();
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        HE_PROFILE_FUNCTION();

        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(1, &m_ColorAttachment);
        glDeleteTextures(1, &m_DepthAttachment);
    }

    void OpenGLFrameBuffer::Invalidate()
    {
        HE_PROFILE_FUNCTION();

        if (m_RendererID != 0)
        {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(1, &m_ColorAttachment);
            glDeleteTextures(1, &m_DepthAttachment);
        }

        glGenFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        glGenTextures(1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,  m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

        glGenTextures(1, &m_DepthAttachment);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8,  m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
        HE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
    {
        m_Specification.Width = width;
        m_Specification.Height = height;

        Invalidate();
    }

    const FrameBufferSpecification& OpenGLFrameBuffer::GetSpecification() const
    {
        return m_Specification;
    }

    uint32_t OpenGLFrameBuffer::GetColorAttachmentRendererID() const
    {
        return m_ColorAttachment;
    }

    void OpenGLFrameBuffer::Bind()
    {
        HE_PROFILE_FUNCTION();

        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, m_Specification.Width, m_Specification.Height);

    }

    void OpenGLFrameBuffer::UnBind()
    {
        HE_PROFILE_FUNCTION();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }


}

