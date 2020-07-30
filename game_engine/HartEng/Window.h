#ifndef WINDOW_H
#define WINDOW_H

#include "HartEng/pch.h"
#include "HartEng/Log.h"
#include "HartEng/Core.h"
#include "HartEng/Events/Event.h"

namespace HE
{
    struct WindowProps
    {
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        WindowProps(const std::string& title = "HartEngine",
                    unsigned int width = 1280,
                    unsigned int height = 720):
            Title(title), Width(width), Height(height) {}
    };

    // Интерфейс для ПК окна
    class Window
    {
    public:
        using EventCallBackFn = std::function<void(Event&)>;

        virtual ~Window() {}

        virtual void OnUpdate() = 0;

        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;

        // Атрибуты окна
        virtual void SetEventCallback(const EventCallBackFn& callback) = 0;
        virtual void SetVSync(bool enable) = 0;
        virtual bool IsSync() const = 0;

        static Window* Create(const WindowProps& props = WindowProps());
    };
}
#endif // WINDOW_H
