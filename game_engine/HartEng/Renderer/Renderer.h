#ifndef RENDERER_H
#define RENDERER_H

#include "HartEng/pch.h"

namespace HE
{
    enum class RendererAPI
    {
        None = 0, OpenGL = 1
    };

    class Renderer
    {
    private:
        static RendererAPI s_RendererAPI;
    public:

        inline static RendererAPI GetRendererAPI()
        {
            return s_RendererAPI;
        }

        inline static void SetRendererAPI(RendererAPI renderer)
        {
            s_RendererAPI =  renderer;
        }



    };


}

#endif // RENDERER_H

