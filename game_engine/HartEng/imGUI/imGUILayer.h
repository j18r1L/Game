#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include "HartEng/pch.h"

#include "HartEng/Layer.h"
#include "HartEng/Application.h"

#include "HartEng/Events/MousesEvent.h"
#include "HartEng/Events/KeyEvent.h"
#include "HartEng/Events/ApplicationEvent.h"

#include "imgui.h"
#include "examples/imgui_impl_opengl3.h"

namespace HE
{
    class ImGuiLayer: public Layer
    {
    private:
        float m_Time = 0.;

        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
        bool OnMouseMovedEvent(MouseMovedEvent& e);
        bool OnMouseScrolledEvent(MouseScrolledEvent& e);

        bool OnKeyPressedEvent(KeyPressedEvent& e);
        bool OnKeyReleasedEvent(KeyReleasedEvent& e);
        bool OnKeyTypedEvent(KeyTypedEvent& e);

        bool OnWindwResizedEvent(WindowResizeEvent& e);
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
