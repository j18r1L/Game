#include "OpenGLFrameBuffer.h"
#include "HartEng/pch.h"

#include "HartEng/Core.h"
#include "HartEng/Log.h"

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
    }

    void OpenGLFrameBuffer::Invalidate()
    {
        HE_PROFILE_FUNCTION();

        glCreateFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, m_Specification.Width, m_Specification.Height);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
        //glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Specification.Width, m_Specification.Height, GL_DEPTH24_STENCIL8, GL_UNSIGNED_BYTE, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

        HE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

    }

    void OpenGLFrameBuffer::UnBind()
    {
        HE_PROFILE_FUNCTION();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }


}

