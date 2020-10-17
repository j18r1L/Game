#include "HartEng/ECS/Components.h"

namespace HE
{
    Component::~Component()
    {
        m_EntityHandle->RemoveComponent(m_Type);
    }

    ComponentType Component::getType()
    {
        return m_Type;
    }



    TransformComponent::TransformComponent(Entity* entityHandle):
        m_Transform(1.0f)
    {
        m_EntityHandle = entityHandle;
        m_Type = ComponentType::TransformComponent;
    }

    TransformComponent::TransformComponent(Entity* entityHandle, const glm::mat4& transform):
        m_Transform(transform)
    {
        m_EntityHandle = entityHandle;
        m_Type = ComponentType::TransformComponent;
    }

    const glm::mat4& TransformComponent::GetTransform()
    {
        return m_Transform;
    }

    // хз нужен ли здесь OnUpdate, так как просто хранятся данные
    void TransformComponent::OnUpdate()
    {

    }


    GraphicsComponent::GraphicsComponent(Entity* entityHandle):
        m_Color(1.0f)
    {
        m_EntityHandle = entityHandle;
        m_Type = ComponentType::GraphicsComponent;
    }

    GraphicsComponent::GraphicsComponent(Entity* entityHandle, const glm::vec4& color):
        m_Color(color)
    {
        m_EntityHandle = entityHandle;
        m_Type = ComponentType::GraphicsComponent;
    }

    const glm::vec4& GraphicsComponent::GetColor()
    {
        return m_Color;
    }

    void GraphicsComponent::OnUpdate()
    {
        std::cout << "Graphics: " << m_Color.r << ", " << m_Color.g << ", " << m_Color.b << std::endl;
        // Допустим нужно узнать X из Transform компонента:
        float x = dynamic_cast<TransformComponent&>(m_EntityHandle->GetComponent(ComponentType::TransformComponent)).GetTransform()[0][0];
        std::cout << "transform_x from Graphics: " << x << std::endl;
    }
}
