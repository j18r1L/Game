#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include "HartEng/pch.h"

#include "HartEng/Layer.h"


#include "HartEng/Events/MousesEvent.h"
#include "HartEng/Events/KeyEvent.h"
#include "HartEng/Events/ApplicationEvent.h"

#include <imgui.h>
#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_glfw.h>

namespace HE
{
    class ImGUILayer: public Layer
    {
    private:
        float m_Time = 0.;
    public:
        ImGUILayer();
        ~ImGUILayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;
        //void OnEvent(Event &event);

        void Begin();
        void End();
    };
}

#endif // IMGUILAYER_H
