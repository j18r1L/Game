#include "HartEng/Core/LayerStack.h"

namespace HE
{
    LayerStack::LayerStack()
    {
    }

    // TODO Сейчас Слои живут на протяжении всего приложения, однако стоит задуматься об их обновлении при, допустим, смене уровня
    LayerStack::~LayerStack()
    {
        for (int i = 0; i < m_Layers.size(); i++)
        {
            m_Layers[i]->OnDetach();
            delete m_Layers[i];
        }
    }
            

    void LayerStack::PushLayer(Layer *layer)
    {
        // Слои пушатся в первую часть вектора, порядок нужно соблюдать, чтобы слои рендерились в парвилном порядке
        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
        m_LayerInsertIndex++;

    }

    void LayerStack::PushOverlay(Layer *overlay)
    {
        // Оверлеи пушатся во вторую часть вектора, порядок нужно соблюдать, чтобы слои рендерились в парвилном порядке
        m_Layers.emplace_back(overlay);
    }

    void LayerStack::PopLayer(Layer *layer)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (it != m_Layers.end())
        {
            layer->OnDetach();
            m_Layers.erase(it);
            m_LayerInsertIndex--;
        }
    }

    void LayerStack::PopOverlay(Layer *overlay)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
        if (it != m_Layers.end())
        {
            overlay->OnDetach();
            m_Layers.erase(it);
        }
    }

}
