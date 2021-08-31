#pragma once



#include "HartEng/Core/Window.h"
#include "HartEng/Renderer/GraphicsContext.h"


#include "HartEng/Core/pch.h"
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
            unsigned int XPosition, YPosition;
            bool VSync;

            EventCallBackFn EventCallback;
        };

        WindowData m_Data;

        virtual void Init(const WindowProps& props);
        virtual void Shutdown();

    public:
        LinuxWindow(const WindowProps& props);
        virtual ~LinuxWindow();


        static void GLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos);

        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }
        inline unsigned int GetXPosition() const override { return m_Data.XPosition; }
        inline unsigned int GetYPosition() const override { return m_Data.YPosition; }
        double GetTime() const override;

        // Window attributes
        void DisableCursor() override;
        void HideCursor() override;
        void ShowCursor() override;
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