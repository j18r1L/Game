#pragma once
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
        virtual void SetMouseXImpl(float mousePosX) override;
        virtual void SetMouseYImpl(float mousePosY) override;
        virtual void SetMousePositionImpl(float mousePosX, float mousePosY) override;
    };
}

