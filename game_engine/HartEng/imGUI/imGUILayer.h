#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include "HartEng/pch.h"

#include "HartEng/Layer.h"




#include <imgui.h>


namespace HE
{
    class ImGUILayer: public Layer
    {
    public:
        ImGUILayer();
        ~ImGUILayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        //void OnEvent(Event &event);

        void Begin();
        void End();
    };
}

#endif // IMGUILAYER_H
