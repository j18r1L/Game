#ifndef LINUXINPUT_H
#define LINUXINPUT_H

#include "HartEng/Core/pch.h"
#include "HartEng/Core/Input.h"


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
