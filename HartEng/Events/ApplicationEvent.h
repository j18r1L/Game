#ifndef APPLICATIONEVENT_H
#define APPLICATIONEVENT_H

#include "HartEng/Events/Event.h"

namespace HE
{
    class WindowResizeEvent: public Event
    {
    private:
        unsigned int m_Width, m_Height; // текущая ширина/высота окна

    public:
        WindowResizeEvent(unsigned int width, unsigned int height):
            m_Width(width),
            m_Height(height) {}

        inline unsigned int GetWidth() const
        {
            return m_Width;
        }

        inline unsigned int GetHeight() const
        {
            return m_Height;
        }

        std::string ToString() const override
        {
            // ! stringstream медленный, не использовать в release, это debug only
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class WindowCloseEvent: public Event
    {
    public:
        WindowCloseEvent() {}

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    // Скорее всего использовать не буду, это если update и render делать эвентами (в идеале все должно быть эвентами)
    class AppTickEvent: public Event
    {
    public:
        AppTickEvent() {}

        EVENT_CLASS_TYPE(AppTick)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AppUpdateEvent: public Event
    {
    public:
        AppUpdateEvent() {}

        EVENT_CLASS_TYPE(AppUpdate)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AppRenderEvent: public Event
    {
    public:
        AppRenderEvent() {}

        EVENT_CLASS_TYPE(AppRender)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
}

#endif // APPLICATIONEVENT_H
