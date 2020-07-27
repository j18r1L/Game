#ifndef APPLICATION_H
#define APPLICATION_H

#include "Core.h"

namespace HE
{
    class Application
    {
    public:
        Application();
        virtual ~Application();
        void Run();
    };

    // To be defined in a client
    Application* CreateApplication();

}

#endif // APPLICATION_H
