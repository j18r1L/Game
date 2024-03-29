#ifndef GRAPHICSCONTEXT_H
#define GRAPHICSCONTEXT_H

namespace HE
{
    class GraphicsContext
    {
    private:

    public:
        virtual ~GraphicsContext() = default;
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;

    };
}

#endif // GRAPHICSCONTEXT_H
