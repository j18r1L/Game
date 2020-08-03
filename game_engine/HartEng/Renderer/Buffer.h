#ifndef BUFFER_H
#define BUFFER_H

#include "HartEng/pch.h"
#include "HartEng/Renderer/Renderer.h"
#include "HartEng/Core.h"
#include "HartEng/Log.h"

#ifdef HE_PLATFORM_WINDOW
#include "HartEng/Platform/OpenGL/OpenGLBuffer.h"
#endif
#include "HartEng/Platform/OpenGL/OpenGLBuffer.h"

namespace HE
{
    class VertexBuffer
    {
    private:

    public:
        virtual ~VertexBuffer() {}

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        static VertexBuffer* Create(float* vertices, uint32_t size);
    };

    class IndexBuffer
    {
    private:

    public:
        virtual ~IndexBuffer() {}

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        static IndexBuffer* Create(uint32_t* indices, uint32_t size);
    };
}

#endif // BUFFER_H

