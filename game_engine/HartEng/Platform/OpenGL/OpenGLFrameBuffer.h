#ifndef OPENGLFRAMEBUFFER_H
#define OPENGLFRAMEBUFFER_H

#include "HartEng/Renderer/Framebuffer.h"

namespace HE
{
    class OpenGLFrameBuffer: public FrameBuffer
    {
    private:
        uint32_t m_RendererID;

        uint32_t m_ColorAttachment;
        uint32_t m_DepthAttachment;

        FrameBufferSpecification m_Specification;



    public:
        ~OpenGLFrameBuffer();
        OpenGLFrameBuffer(const FrameBufferSpecification& spec);

        uint32_t GetColorAttachmentRendererID() const override;

        void Invalidate();

        virtual void Bind() override;
        virtual void UnBind() override;

        const FrameBufferSpecification & GetSpecification() const override;
    };

}

#endif // OPENGLFRAMEBUFFER_H
