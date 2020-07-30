#ifndef LAYER_H
#define LAYER_H

#include "HartEng/Core.h"
#include "HartEng/pch.h"
#include "HartEng/Events/Event.h"

namespace HE
{
    class Layer
    {
    protected:
        std::string m_DebugName;
    public:
        // Создаем слой, TODO так-то слои не должны иметь имени в release build-e, используются для дебага
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        // Когда слой добавляется в LayerStack
        virtual void OnAttach() {}
        // Когда слой удаляется из LayerStack
        virtual void OnDetach() {}
        // Вызывается когда обновляются слои (на каждом кадре)
        virtual void OnUpdate() {}
        // Когда эвент посылвается в слой мы его принимаем здесь
        virtual void OnEvent(Event& event) {}

        // TODO добавить включение и выключение слоев испльзуя bool он не будет вызывать Onupdate


        inline const std::string& GetName() const
        {
            return m_DebugName;
        }
    };
}


#endif // LAYER_H
