#ifndef INPUTCOMPONENTS_H
#define INPUTCOMPONENTS_H

#include "CameraComponents.h"

class InputComponent
{
public:
    inline static bool m_keys[1024];

    inline static GLdouble m_xoffset_scroll;
    inline static GLdouble m_yoffset_scroll;

    inline static GLfloat m_xoffset_mouse;
    inline static GLfloat m_yoffset_mouse;
    inline static GLfloat m_lastX_mouse;
    inline static GLfloat m_lastY_mouse;

    inline static bool m_first_mouse;
    InputComponent();
    virtual ~InputComponent() {};
    virtual void update(GLfloat deltaFrameTime) = 0;

    static void key_callback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mode);
    static void scroll_callback(GLFWwindow* window, GLdouble xoffset, GLdouble yoffset);
    static void mouse_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos);
};

class PlayerInputComponent : public InputComponent
{
private:
    CameraComponent* m_CameraComponent;
public:
    PlayerInputComponent(CameraComponent* cameraComponent):
        m_CameraComponent(cameraComponent)  {}
    ~PlayerInputComponent()
    {
        delete m_CameraComponent;
    }

    virtual void update(GLfloat deltaFrameTime);


};

#endif // INPUTCOMPONENTS_H
