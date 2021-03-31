#pragma once

#include "HartEng/Core/pch.h"
#include "HartEng/Core/Log.h"
#include "HartEng/Core/Core.h"
#include "HartEng/Events/Event.h"

namespace HE
{
    struct WindowProps
    {
        std::string Title;
        uint32_t Width;
        uint32_t Height;
        uint32_t XPosition;
        uint32_t YPosition;

        WindowProps(const std::string& title = "HartEngine",
                    uint32_t width = 1280,
                    uint32_t height = 1020,
                    uint32_t xPosition = 100,
                    uint32_t yPosition = 100):
            Title(title), 
            Width(width), 
            Height(height),
            XPosition(xPosition),
            YPosition(yPosition)
        {
        }
    };

    // Интерфейс для ПК окна
    class Window
    {
    public:
        using EventCallBackFn = std::function<void(Event&)>;

        virtual ~Window() {}

        virtual void OnUpdate() = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual uint32_t GetXPosition() const = 0;
        virtual uint32_t GetYPosition() const = 0;
        virtual double GetTime() const = 0;

        // Атрибуты окна
        virtual void DisableCursor() = 0;
        virtual void HideCursor() = 0;
        virtual void ShowCursor() = 0;
        virtual void SetEventCallback(const EventCallBackFn& callback) = 0;
        virtual void SetVSync(bool enable) = 0;
        virtual bool IsSync() const = 0;

        // Так как это не обязательно GLFW окно, а может быть и win32, то используется void*
        virtual void* GetNativeWindow() const = 0;

        static Window* Create(const WindowProps& props = WindowProps());
    };
}
