#pragma once

#include "HartEng/Core/pch.h"
#include <glm/glm.hpp>

namespace HE
{
    // Do we need color att?
    // Do we need DepthAttachment 
    // internalformat: GL_RGBA / GL_R32I / GL_DEPTH24_STENCIL8
    // format: GL_RGBA / GL_RED_INTEGER / GL_DEPTH_STENCIL
    // type: GL_UNSIGNED_BYTE / GL_UNSIGNED_INT_24_8
    // GL_LINEAR / GL_NEAREST
    enum class FramebufferTextureFormat
    {
        None = 0,

        // Color
        R8      = 1,
        R32I    = 2,
        RGBA8   = 3,
        RGBA16F = 4,
        RGBA32F = 5,
        RG16F   = 6,
        RG32F   = 7,

        // Depth/Stencil
        DEPTH32F = 8,
        DEPTH24STENCIL8 = 9,

        // Defaults
        DEPTH = DEPTH24STENCIL8
    };

    enum class FramebufferTextureFilter
    {
        NEAREST = 0,
        LINEAR = 1,
    };

    enum class FramebufferBindType
    {
        FRAMEBUFFER = 0,
        DRAW_FRAMEBUFFER = 1,
        READ_FRAMEBUFFER = 2,
    };

    struct FramebufferTextureSpecification
    {
        FramebufferTextureSpecification() = default;
        FramebufferTextureSpecification(FramebufferTextureFormat format, FramebufferTextureFilter filter = FramebufferTextureFilter::LINEAR) :
            TextureFormat(format),
            TextureFilter(filter)
        {}

        FramebufferTextureFormat TextureFormat;
        FramebufferTextureFilter TextureFilter;
    };
    struct FramebufferAttachmentSpecification
    {
        FramebufferAttachmentSpecification() = default;
        FramebufferAttachmentSpecification(const std::initializer_list<FramebufferTextureSpecification>& attachments) :
            Attachments(attachments) {}

        std::vector<FramebufferTextureSpecification> Attachments;
    };

    struct FrameBufferSpecification
    {
        // Struct of properties
        uint32_t Width = 0.0f, Height = 0.0f;
        glm::vec4 ClearColor{ 1.0f, 0.0f, 1.0f, 1.0f };
        FramebufferAttachmentSpecification Attachemtns;
        uint32_t Samples = 1;

        bool SwapChainTarget = false;

    };

    class FrameBuffer
    {
    public:
        virtual ~FrameBuffer() = default;

        virtual uint32_t GetColorAttachmentRendererID(int index = 0) const = 0;
        virtual const FrameBufferSpecification& GetSpecification() const = 0;

        // TODO убрать, когда будет хороший рендерер
        virtual void Bind(FramebufferBindType type = FramebufferBindType::FRAMEBUFFER) = 0;
        virtual void UnBind() = 0;
        virtual void Resize(uint32_t width, uint32_t height) = 0;



        static std::shared_ptr<FrameBuffer> Create(const FrameBufferSpecification& spec);
    };
}