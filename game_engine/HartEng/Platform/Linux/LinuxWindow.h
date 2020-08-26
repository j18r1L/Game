#ifndef LINUXWINDOW_H
#define LINUXWINDOW_H



#include "HartEng/Window.h"
#include "HartEng/Renderer/GraphicsContext.h"


#include "HartEng/pch.h"
#include <GLFW/glfw3.h>

// GLFW platform specific code
namespace HE
{


    // Интерфейс для ПК окна
    class LinuxWindow: public Window
    {
    private:
        GLFWwindow* m_Window;
        GraphicsContext* m_Context;

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
        static void GLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        LinuxWindow(const WindowProps& props);
        virtual ~LinuxWindow();

        // Вызывается каждый кадр
        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }
        double GetTime() const override;

        // Атрибуты окна
        inline void SetEventCallback(const EventCallBackFn& callback) override
        {
            m_Data.EventCallback = callback;
        };
        void SetVSync(bool enable) override;
        bool IsSync() const override;

        inline virtual void* GetNativeWindow() const
        {
            return m_Window;
        }


        static Window* Create(const WindowProps& props = WindowProps());
    };

    // Callback function
    static void GLFWErrorCallback(int error, const char* description);


}
#endif // LINUXWINDOW_H
