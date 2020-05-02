#include "InputComponents.h"

InputComponent::InputComponent()
{
    VideoSettings* videoSettings = VideoSettings::getInstance();
    GLint width, height;
    videoSettings->getWidthHeight(&width, &height);
    m_lastX_mouse = static_cast<GLfloat>(width) / 2.0f;
    m_lastY_mouse = static_cast<GLfloat>(height) / 2.0f;
    m_first_mouse = true;
}


void InputComponent::key_callback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if(action == GLFW_PRESS)
            m_keys[key] = true;
        else if(action == GLFW_RELEASE)
            m_keys[key] = false;
    }
}

void InputComponent::scroll_callback(GLFWwindow* window, GLdouble xoffset, GLdouble yoffset)
{
    m_xoffset_scroll = xoffset;
    m_yoffset_scroll = yoffset;
}
void InputComponent::mouse_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos)
{
    if(m_first_mouse)
    {
        m_lastX_mouse = xpos;
        m_lastY_mouse = ypos;
        m_first_mouse = false;
    }

    m_xoffset_mouse = xpos - m_lastX_mouse;
    m_yoffset_mouse = m_lastY_mouse - ypos;

    m_lastX_mouse = xpos;
    m_lastY_mouse = ypos;
}

void PlayerInputComponent::update(GLfloat deltaFrameTime)
{
    Camera* camera = m_CameraComponent->getCamera();
    if(m_keys[GLFW_KEY_W])
        camera->ProcessKeyboard(FORWARD, deltaFrameTime);
    if(m_keys[GLFW_KEY_S])
        camera->ProcessKeyboard(BACKWARD, deltaFrameTime);
    if(m_keys[GLFW_KEY_A])
        camera->ProcessKeyboard(LEFT, deltaFrameTime);
    if(m_keys[GLFW_KEY_D])
        camera->ProcessKeyboard(RIGHT, deltaFrameTime);
    if(m_keys[GLFW_KEY_Q])
        camera->ProcessKeyboard(UP, deltaFrameTime);
    if(m_keys[GLFW_KEY_E])
        camera->ProcessKeyboard(DOWN, deltaFrameTime);
    if(m_keys[GLFW_KEY_O])
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if(m_keys[GLFW_KEY_P])
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    camera->ProcessMouseMovement(m_xoffset_mouse, m_yoffset_mouse);
    camera->ProcessMouseScroll(m_yoffset_scroll);
    m_xoffset_mouse = 0;
    m_yoffset_mouse = 0;
    m_yoffset_scroll = 0;
}
