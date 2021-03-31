#pragma once

#include "HartEng/Core/pch.h"

#include "HartEng/Core/Layer.h"




#include <imgui.h>


namespace HE
{
    class ImGUILayer: public Layer
    {
    private:
        bool m_BlockEvents = false;
    public:
        ImGUILayer();
        ~ImGUILayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        //void OnEvent(Event &event);

        void Begin();
        void End();
        void SetBlockEvents(bool block)
        {
            m_BlockEvents = block;
        }
    };
}
