#ifndef LINUXWINDOW_H
#define LINUXWINDOW_H


#include "HartEng/Window.h"

#include "HartEng/pch.h"

// GLFW platform specific code
namespace HE
{
    // Интерфейс для ПК окна
    class LinuxWindow: public Window
    {
    private:
        GLFWwindow* m_Window;

        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallBackFn EventCallback;
        };

        WindowData m_Data;

        virtual void Init(const WindowProps& props);
        virtual void Shutdown();

    public:
        LinuxWindow(const WindowProps& props);
        virtual ~LinuxWindow();

        // Вызывается каждый кадр
        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }

        // Атрибуты окна
        inline void SetEventCallback(const EventCallBackFn& callback) override
        {
            m_Data.EventCallback = callback;
        };
        void SetVSync(bool enable) override;
        bool IsSync() const override;


        static Window* Create(const WindowProps& props = WindowProps());
    };
}
#endif // LINUXWINDOW_H
