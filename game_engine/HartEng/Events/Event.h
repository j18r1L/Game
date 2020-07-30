#ifndef EVENT_H
#define EVENT_H

#include "HartEng/pch.h"
#include "HartEng/Core.h"


namespace HE
{
// Макросы нужны чтобы имплементировать  (GetEventType, GetName, GetCategoryFlags), не копипастить код
#define EVENT_CLASS_TYPE(type) \
    static EventType GetStaticType() {return EventType::type; } \
    virtual EventType GetEventType() const override { return GetStaticType(); } \
    virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
    virtual int GetCategoryFlags() const override {return category; }


    // ! Возникающие эвенты обрабатываются сразу, в будущем надо сделать буффер

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
        // Так как один эвент может входить в несколько категорий эвентов
        None = 0,
        EventCategoryApplication =  BIT(0),
        EventCategoryInput =        BIT(1),
        EventCategoryKeyboard =     BIT(2),
        EventCategoryMouse =        BIT(3),
        EventCategoryMouseButton =  BIT(4)
    };

    
    class Event
    {
        //friend class EventDispatcher;

    public:
        // Был ли обработан эвент или нет, нужен для того чтобы предотварить эвент от падения ниже по слоям обработки игры
        bool Handled = false;

        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0; 
        virtual std::string ToString() const 
        {
            return GetName();
        }
        // Принадлежит ли этот эвент данной категории
        // 0 - False, Other - true
        inline bool IsInCategory(EventCategory category)
        {
            return GetCategoryFlags()& category;
        }

    };
    
    
    
    // Используем тип эвентов для отправки эвентов
    class EventDispatcher
    {
        template<typename T>
        using EventFn = std::function<bool(T&)>; // T - любой тип эвента
    private:
        Event& m_Event;

    public:
        // На вход любой тип эвента, создаем объект класса с текущим эвентом и потом вызываем Dispatch функцию с разной Event функцией
        EventDispatcher(Event& event):
            m_Event(event) {}

        template<typename T>
        bool Dispatch(EventFn<T> func)
        {
            // Если эвент, который отправляем совпадает с типом EventFn, то вызывается эта функция
            if (m_Event.GetEventType() == T::GetStaticType())
            {
                m_Event.Handled = func(*(T*)&m_Event);
                return true;
            }
            return false;
        }

    };

    inline std::ostream& operator << (std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }
}






#endif // EVENT_H
