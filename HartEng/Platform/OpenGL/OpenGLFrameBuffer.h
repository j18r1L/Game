#pragma once

#include "HartEng/Renderer/Framebuffer.h"

namespace HE
{
    class OpenGLFrameBuffer: public FrameBuffer
    {
    private:
        std::vector<uint32_t> m_Attachments;
        uint32_t m_RendererID = 0;

        uint32_t m_ColorAttachmentSize = 0;

        FrameBufferSpecification m_Specification;



    public:
        ~OpenGLFrameBuffer();
        OpenGLFrameBuffer(const FrameBufferSpecification& spec);

        uint32_t GetColorAttachmentRendererID(int index = 0) const override;

        void Invalidate();

        void Bind(FramebufferBindType type = FramebufferBindType::FRAMEBUFFER) const override;
        void UnBind() const override;
        void Resize(uint32_t width, uint32_t height) override;
        
        int ReadPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y) const override;

        const FrameBufferSpecification & GetSpecification() const override;
    };

}
