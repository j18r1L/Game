#include "VideoSettings.h"

void VideoSettings::setWidth(GLint* width)
{
    m_Width = *width;
}

void VideoSettings::setHeight(GLint* height)
{
    m_Height = *height;
}

void VideoSettings::setWidthHeight(GLint* width, GLint* height)
{
    m_Width = *width;
    m_Height = *height;
}

void VideoSettings::setWidthHeight(GLint width, GLint height)
{
    m_Width = width;
    m_Height = height;
}

void VideoSettings::getWidth(GLint* width) const
{
    *width = m_Width;
}

void VideoSettings::getHeight(GLint* height) const
{
    *height = m_Height;
}

void VideoSettings::getWidthHeight(GLint* width, GLint* height) const
{
    *width = m_Width;
    *height = m_Height;
}

void VideoSettings::getFullScreen(bool* FullScreen) const
{
    *FullScreen = m_FullScreen;
}
