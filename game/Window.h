#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
private:
    GLFWwindow* m_Window;   // Окно
    
public:
    // Пустой конструктор
    Window() {}
    
    // Создаем новое окно, заполняем width height
    Window(GLint width, GLint height, bool full_screen);
    
    // Удаляем текущее окно
    ~Window() {}
    // Устанавливаем контекст
    void MakeContextCurrent() const;
    // Устанавливаем функцию обработки ввода
    void setKeyCallback(GLFWkeyfun key_callback);
    void setCursorPosCallback(GLFWcursorposfun mouse_callback);
    void setScrollCallback(GLFWscrollfun scroll_callback);

    // Возвращаем окно
    GLFWwindow* getWindow() const
    {
        return m_Window;
    }
};

#endif // WINDOW_H
