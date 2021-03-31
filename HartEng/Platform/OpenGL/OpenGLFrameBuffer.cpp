#include "OpenGLFrameBuffer.h"
#include "HartEng/Core/pch.h"

#include "HartEng/Core/Core.h"
#include "HartEng/Core/Log.h"
#include "HartEng/Renderer/Renderer.h"
#include <glad/glad.h>

namespace HE
{
    static GLenum FramebufferType(FramebufferBindType type)
    {
        switch (type)
        {
        case FramebufferBindType::FRAMEBUFFER: return GL_FRAMEBUFFER;
        case FramebufferBindType::DRAW_FRAMEBUFFER: return GL_DRAW_FRAMEBUFFER;
        case FramebufferBindType::READ_FRAMEBUFFER: return GL_READ_FRAMEBUFFER;
        }
    }
    static GLenum InternalFormatType(FramebufferTextureFormat format)
    {
        switch (format)
        {
        case FramebufferTextureFormat::R8:              return GL_R8;
        case FramebufferTextureFormat::R32I:            return GL_R32I;
        case FramebufferTextureFormat::RG16F:           return GL_RG16F;
        case FramebufferTextureFormat::RG32F:           return GL_RG32F;
        case FramebufferTextureFormat::RGBA8:           return GL_RGBA8;
        case FramebufferTextureFormat::RGBA16F:         return GL_RGBA16F;
        case FramebufferTextureFormat::RGBA32F:         return GL_RGBA32F;
        case FramebufferTextureFormat::DEPTH24STENCIL8: return GL_DEPTH24_STENCIL8;
        }
        HE_CORE_ASSERT(false, "Unknown format!");
        return 0;
    }

    static GLenum FormatType(FramebufferTextureFormat format)
    {
        switch (format)
        {
        case FramebufferTextureFormat::R8:              return GL_RED;
        case FramebufferTextureFormat::R32I:            return GL_RED_INTEGER;
        case FramebufferTextureFormat::RG16F:
        case FramebufferTextureFormat::RG32F:           return GL_RG;
        case FramebufferTextureFormat::RGBA8:
        case FramebufferTextureFormat::RGBA16F:
        case FramebufferTextureFormat::RGBA32F:         return GL_RGBA;
        case FramebufferTextureFormat::DEPTH24STENCIL8: return GL_DEPTH_STENCIL;
        }
        HE_CORE_ASSERT(false, "Unknown format!");
        return 0;
    }

    static GLenum DataType(FramebufferTextureFormat format)
    {
        switch (format)
        {
        case FramebufferTextureFormat::R8:             
        case FramebufferTextureFormat::R32I:       
        case FramebufferTextureFormat::RGBA8:           return GL_UNSIGNED_BYTE;
        case FramebufferTextureFormat::RG16F:
        case FramebufferTextureFormat::RG32F:
        case FramebufferTextureFormat::RGBA16F:
        case FramebufferTextureFormat::RGBA32F:         return GL_FLOAT;
        case FramebufferTextureFormat::DEPTH24STENCIL8: return GL_UNSIGNED_INT_24_8;
        }
        HE_CORE_ASSERT(false, "Unknown format!");
        return 0;
    }

    static GLenum FilterType(FramebufferTextureFilter filter)
    {
        switch (filter)
        {
        case FramebufferTextureFilter::LINEAR: return GL_LINEAR;
        case FramebufferTextureFilter::NEAREST: return GL_NEAREST;
        }
        HE_CORE_ASSERT(false, "Unknown filter format!");
        return 0;
    }

    static GLenum ColorAttachment(uint32_t attachment, FramebufferTextureFormat format)
    {
        if (format == FramebufferTextureFormat::DEPTH24STENCIL8)
        {
            return GL_DEPTH_STENCIL_ATTACHMENT;
        }
        else if (format == FramebufferTextureFormat::DEPTH32F)
        {
            return GL_DEPTH_ATTACHMENT;
        }
        else
        {
            switch (attachment)
            {
            case 0: return GL_COLOR_ATTACHMENT0;
            case 1: return GL_COLOR_ATTACHMENT1;
            case 2: return GL_COLOR_ATTACHMENT2;
            case 3: return GL_COLOR_ATTACHMENT3;
            case 4: return GL_COLOR_ATTACHMENT4;
            case 5: return GL_COLOR_ATTACHMENT5;
            case 6: return GL_COLOR_ATTACHMENT6;
            case 7: return GL_COLOR_ATTACHMENT7;
            case 8: return GL_COLOR_ATTACHMENT8;
            case 9: return GL_COLOR_ATTACHMENT9;
            case 10: return GL_COLOR_ATTACHMENT10;
            case 11: return GL_COLOR_ATTACHMENT11;
            case 12: return GL_COLOR_ATTACHMENT12;
            case 13: return GL_COLOR_ATTACHMENT13;
            case 14: return GL_COLOR_ATTACHMENT14;
            case 15: return GL_COLOR_ATTACHMENT15;
            case 16: return GL_COLOR_ATTACHMENT16;
            case 17: return GL_COLOR_ATTACHMENT17;
            case 18: return GL_COLOR_ATTACHMENT18;
            case 19: return GL_COLOR_ATTACHMENT19;
            case 20: return GL_COLOR_ATTACHMENT20;
            case 21: return GL_COLOR_ATTACHMENT21;
            case 22: return GL_COLOR_ATTACHMENT22;
            case 23: return GL_COLOR_ATTACHMENT23;
            case 24: return GL_COLOR_ATTACHMENT24;
            case 25: return GL_COLOR_ATTACHMENT25;
            case 26: return GL_COLOR_ATTACHMENT26;
            case 27: return GL_COLOR_ATTACHMENT27;
            case 28: return GL_COLOR_ATTACHMENT28;
            case 29: return GL_COLOR_ATTACHMENT29;
            case 30: return GL_COLOR_ATTACHMENT30;
            case 31: return GL_COLOR_ATTACHMENT31;
            }
            HE_CORE_ASSERT(false, "Too many color attachments!");
            return 0;
        }
        HE_CORE_ASSERT(false, "Unknown attachment!");
        return 0;
    }

    static GLenum TextureTarget(uint32_t samples)
    {
        return samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec):
        m_Specification(spec)
    {
        Renderer::Submit([this]() mutable
            {
                Invalidate();
            });
        
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        HE_PROFILE_FUNCTION();
        Renderer::Submit([this]()
            {
                glDeleteFramebuffers(1, &this->m_RendererID);
                for (int i = 0; i < this->m_Attachments.size(); i++)
                {
                    glDeleteTextures(1, &this->m_Attachments[i]);
                }
            });
    }

    // Store this function in Renderer::Submit, the function itself does not submitting to Renderer
    void OpenGLFrameBuffer::Invalidate()
    {
        HE_PROFILE_FUNCTION();
        
        if (m_RendererID != 0)
        {
            glDeleteFramebuffers(1, &m_RendererID);
            for (int i = 0; i < m_Attachments.size(); i++)
            {
                glDeleteTextures(1, &m_Attachments[i]);
            }
        }
        else
        {
            m_Attachments.resize(m_Specification.Attachments.Attachments.size());
            for (auto& attachment : m_Specification.Attachments.Attachments)
            {
                if (attachment.TextureFormat != FramebufferTextureFormat::DEPTH24STENCIL8 && attachment.TextureFormat != FramebufferTextureFormat::DEPTH32F)
                {
                    m_ColorAttachmentSize++;
                }
            }
        }

        glGenFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        int i = 0;
        for (auto& attachment: m_Specification.Attachments.Attachments)
        {
            glGenTextures(1, &m_Attachments[i]);
            glBindTexture(TextureTarget(m_Specification.Samples), m_Attachments[i]);
            if (m_Specification.Samples > 1)
            {
                glTexImage2DMultisample(
                    GL_TEXTURE_2D_MULTISAMPLE,
                    m_Specification.Samples,
                    InternalFormatType(attachment.TextureFormat),
                    m_Specification.Width,
                    m_Specification.Height,
                    GL_TRUE);
            }
            else
            {
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0, 
                    InternalFormatType(attachment.TextureFormat), 
                    m_Specification.Width, 
                    m_Specification.Height, 
                    0, 
                    FormatType(attachment.TextureFormat), 
                    DataType(attachment.TextureFormat), 
                    nullptr);
            }
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterType(attachment.TextureFilter));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterType(attachment.TextureFilter));
            glFramebufferTexture2D(GL_FRAMEBUFFER, ColorAttachment(i, attachment.TextureFormat), TextureTarget(m_Specification.Samples), m_Attachments[i], 0);
            i++;
        }

        // Attach
        std::vector<GLenum> drawBuffers(m_ColorAttachmentSize);
        for (int j = 0; j < m_ColorAttachmentSize; j++)
        {
            drawBuffers[j] = ColorAttachment(j, m_Specification.Attachments.Attachments[j].TextureFormat);
        }
        glDrawBuffers(m_ColorAttachmentSize, &drawBuffers[0]);
        HE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
    {
        m_Specification.Width = width;
        m_Specification.Height = height;

        Renderer::Submit([this]() mutable
            {
                glViewport(0, 0, this->m_Specification.Width, this->m_Specification.Height);
                this->Invalidate();
            });
    }

    int OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y) const
    {
        // TODO decide do you need to bind and unbind the framebuffer manually or in this function
        HE_CORE_ASSERT(attachmentIndex < m_Attachments.size(), "There is no attachment with index: " + std::to_string(attachmentIndex));
        //glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        int pixelData = -1.0f;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
        HE_CORE_TRACE("pixelData: {0}", pixelData);
        return pixelData;
    }

    const FrameBufferSpecification& OpenGLFrameBuffer::GetSpecification() const
    {
        return m_Specification;
    }

    uint32_t OpenGLFrameBuffer::GetColorAttachmentRendererID(int index) const
    {
        return m_Attachments[index];
    }

    void OpenGLFrameBuffer::Bind(FramebufferBindType type) const
    {
        HE_PROFILE_FUNCTION();

        Renderer::Submit([this, type]()
            {
                glBindFramebuffer(FramebufferType(type), m_RendererID);
            });

    }

    void OpenGLFrameBuffer::UnBind() const 
    {
        HE_PROFILE_FUNCTION();
        Renderer::Submit([]() 
            {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            });

    }


}

