#pragma once

#include "HartEng/Events/Event.h"

namespace HE
{
    // Содержит KeyCode - номер клавиши
    class KeyEvent: public Event
    {
    protected:
        int m_KeyCode;

        KeyEvent(int keycode):
            m_KeyCode(keycode) {}

    public:
        inline int GetKeyCode() const
        {
            return m_KeyCode;
        }

        // keyboardevent тоже является и input event-ом
        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)


    };


    class KeyPressedEvent: public KeyEvent
    {
    private:
         int m_RepeatCount;

    public:
        // repeatCount - количество повторений одной клавиши, если <= 1 - значит клавиша зажата, если 0, то клавиша нажата 1 раз
        KeyPressedEvent(int keycode, int repeatCount):
            KeyEvent(keycode),
            m_RepeatCount(repeatCount) {}

        inline int GetRepeatCount() const
        {
            return m_RepeatCount;
        }

        std::string ToString() const override
        {
            // ! stringstream медленный, не использовать в release, это debug only
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    };

    class KeyReleasedEvent: public KeyEvent
    {
    public:
        KeyReleasedEvent(int keycode):
            KeyEvent(keycode) {}

        std::string ToString() const override
        {
            // ! stringstream медленный, не использовать в release, это debug only
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)

    };

    class KeyTypedEvent: public KeyEvent
    {
    public:
        // repeatCount - количество повторений одной клавиши, если <= 1 - значит клавиша зажата, если 0, то клавиша нажата 1 раз
        KeyTypedEvent(int keycode):
            KeyEvent(keycode) {}

        std::string ToString() const override
        {
            // ! stringstream медленный, не использовать в release, это debug only
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };
}
