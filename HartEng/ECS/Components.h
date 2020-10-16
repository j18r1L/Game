#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "glm/glm.hpp"
#include "HartEng/ECS/Entity.h"
#include "HartEng/Renderer/Cameras.h"

namespace HE
{
    class Entity;

    enum class ComponentType
    {
        UndefinedComponent,
        TransformComponent,
        GraphicsComponent,
    };

    class Component
    {
    protected:
        Entity* m_EntityHandle;
        ComponentType type;
    public:
        virtual ~Component()
        {
            //m_EntityHandle->RemoveComponent();
        }

        virtual void OnUpdate() = 0;

        ComponentType getType()
        {
            return type;
        }
    };

    class TransformComponent: public Component
    {
    private:
        glm::mat4 m_Transform;
    public:
        TransformComponent():
            m_Transform(1.0f)
        {
            type = ComponentType::TransformComponent;
        }

        TransformComponent(const glm::mat4& transform):
            m_Transform(transform)
        {
            type = ComponentType::TransformComponent;
        }

        const glm::mat4& GetTransform();

        // хз нужен ли здесь OnUpdate, так как просто хранятся данные
        void OnUpdate() override;
    };



    class GraphicsComponent: public Component
    {
    private:
        glm::vec4 m_Color;
    public:
        GraphicsComponent():
            m_Color(1.0f)
        {
            type = ComponentType::GraphicsComponent;
        }

        GraphicsComponent(const glm::vec4& color):
            m_Color(color)
        {
            type = ComponentType::GraphicsComponent;
        }

        const glm::vec4& GetColor();

        void OnUpdate() override
        {
            std::cout << "Graphics: " << m_Color.r << ", " << m_Color.g << ", " << m_Color.b << std::endl;
            // Допустим нужно узнать X из Transform компонента:
            //float x = m_EntityHandle->GetComponent(TransformComponent).GetTransform()[0][0];
            //std::cout << "transform_x from Graphics: " << x << std::endl;
        }
    };
}

//class 
#endif // COMPONENTS_H
