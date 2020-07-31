#ifndef LINUXINPUT_H
#define LINUXINPUT_H

#include "HartEng/pch.h"
#include "HartEng/Input.h"
#include "HartEng/Application.h"

namespace HE
{
    class LinuxInput: public Input
    {
    protected:
        virtual bool IsKeyPressedImpl(int keycode) override;
        virtual bool IsMouseButtonPressedImpl(int button) override;
        virtual float GetMouseXImpl() override;
        virtual float GetMouseYImpl() override;
        virtual std::pair<float, float> GetMousePositionImpl() override;
    };
}

#endif // LINUXINPUT_H
