#include <HartEng/HartEng.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

// Entry point
#include "HartEng/Core/EntryPoint.h"

#include "SandBoxLayer.h"

namespace HE
{
    class SandBox: public Application
    {
    public:
        SandBox():
            Application("SandBox")
        {

            PushLayer(new SandBoxLayer());
        }

        ~SandBox() {}
    };


    Application* CreateApplication()
    {
        return new SandBox();

    }
}


