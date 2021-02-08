#pragma once

#include <stdint.h>

namespace HE
{
    class RenderCommandQueue
    {
        typedef void(*RenderCommandFn)(void*);
    private:
        uint8_t* m_CommandBuffer;
        uint8_t* m_CommandBufferPtr;
        uint32_t m_CommandCount = 0;
    public:
        RenderCommandQueue();
        ~RenderCommandQueue();

        void* Allocate(RenderCommandFn func, uint32_t size);
        void Execute();
    };


}


