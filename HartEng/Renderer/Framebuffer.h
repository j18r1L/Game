#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "HartEng/Core/pch.h"
#include <glm/glm.hpp>

namespace HE
{
    struct FrameBufferSpecification
    {
        // Struct of properties
        uint32_t Width, Height;
        //FramebufferFormat Format =
        uint32_t Samples = 1;

        bool SwapChainTarget = false;

    };

    class FrameBuffer
    {
    public:
        virtual ~FrameBuffer() {}

        virtual uint32_t GetColorAttachmentRendererID() const = 0;
        virtual const FrameBufferSpecification& GetSpecification() const = 0;

        // TODO убрать, когда будет хороший рендерер
        virtual void Bind() = 0;
        virtual void UnBind() = 0;
        virtual void Resize(uint32_t width, uint32_t height) = 0;



        static std::shared_ptr<FrameBuffer> Create(const FrameBufferSpecification& spec);
    };
}


#endif // FRAMEBUFFER_H
