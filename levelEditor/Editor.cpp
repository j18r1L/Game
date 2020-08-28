#include <HartEng/HartEng.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

// Entry point
#include "HartEng/Core/EntryPoint.h"
#include "EditorLayer.h"

namespace HE
{
    class LevelEditor: public Application
    {
    public:
        LevelEditor():
            Application("Level Editor")
        {

            PushLayer(new EditorLayer());
        }

        ~LevelEditor() {}
    };


    Application* CreateApplication()
    {
        return new LevelEditor();

    }
}


