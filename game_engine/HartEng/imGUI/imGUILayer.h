#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include "HartEng/Layer.h"
#include "HartEng/pch.h"
#include "HartEng/Application.h"

#include "imgui.h"
#include "examples/imgui_impl_opengl3.h"

namespace HE
{
    class ImGuiLayer: public Layer
    {
    private:
        float m_Time = 0.;
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach();
        void OnDetach();
        void OnUpdate();
        void OnEvent(Event &event);
    };
}

#endif // IMGUILAYER_H
