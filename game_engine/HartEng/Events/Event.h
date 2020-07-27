#ifndef EVENT_H
#define EVENT_H

#include "HartEng/Core.h"

#include <string>
#include <functional>

namespace HE
{
    // Возникающие эвенты обрабатываются сразу, в будущем надо сделать буффер

    // Описывают тип эвента
    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick,
        AppUpdate, AppRender,
        KeyPressed, KeyReleased,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    // Описывают категорию эвентов
    enum EventCategory
    {
        None = 0,
        EventCategoryApplication =  BIT(0),
        EventCategoryInput =        BIT(1),
        EventCategoryKeyboard =     BIT(2),
        EventCategoryMouse =        BIT(3),
        EventCategoryMouseButton =  BIT(4)
    };
}


#endif // EVENT_H
