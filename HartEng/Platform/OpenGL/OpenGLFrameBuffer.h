#ifndef OPENGLFRAMEBUFFER_H
#define OPENGLFRAMEBUFFER_H

#include "HartEng/Renderer/Framebuffer.h"

namespace HE
{
    class OpenGLFrameBuffer: public FrameBuffer
    {
    private:
        std::vector<uint32_t> m_Attachments;
        uint32_t m_RendererID = 0;

        uint32_t m_ColorAttachmentSize = 0;

        //uint32_t m_ColorAttachment = 0;
        //uint32_t m_DepthAttachment = 0;
        //uint32_t m_IDAttachment = 0;

        FrameBufferSpecification m_Specification;



    public:
        ~OpenGLFrameBuffer();
        OpenGLFrameBuffer(const FrameBufferSpecification& spec);

        uint32_t GetColorAttachmentRendererID(int index = 0) const override;

        void Invalidate();

        void Bind(FramebufferBindType type = FramebufferBindType::FRAMEBUFFER) override;
        void UnBind() override;
        void Resize(uint32_t width, uint32_t height) override;
        

        const FrameBufferSpecification & GetSpecification() const override;
    };

}

#endif // OPENGLFRAMEBUFFER_H
