#ifndef SETTINGS_H
#define SETTINGS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class VideoSettings
{
private:
    /* Private constructor to prevent instancing. */
    VideoSettings()
    {
        m_Width = 800;
        m_Height = 600;
        m_FullScreen = false;
    }
    inline static GLint m_Width;
    inline static GLint m_Height;
    inline static bool m_FullScreen;

public:
    /* Static access method. */
    static VideoSettings* getInstance()
    {
        static VideoSettings instance;
        return &instance;
    }
    void setWidth(GLint* width);
    void setHeight(GLint* height);
    void setWidthHeight(GLint* width, GLint* height);
    void setWidthHeight(GLint width, GLint height);

    void getWidth(GLint* width) const;
    void getHeight(GLint* height) const;
    void getWidthHeight(GLint* width, GLint* height) const;
    void getFullScreen(bool* FullScreen) const;
};






#endif // SETTINGS_H
