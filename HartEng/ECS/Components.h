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
        Entity* m_EntityHandle = nullptr;
        ComponentType m_Type = ComponentType::UndefinedComponent;
    public:
        virtual ~Component();

        virtual void OnUpdate() = 0;

        ComponentType getType();
    };

    class TransformComponent: public Component
    {
    private:
        glm::mat4 m_Transform;
    public:
        TransformComponent(Entity* entityHandle);

        TransformComponent(Entity* entityHandle, const glm::mat4& transform);

        const glm::mat4& GetTransform();

        // хз нужен ли здесь OnUpdate, так как просто хранятся данные
        void OnUpdate() override;
    };



    class GraphicsComponent: public Component
    {
    private:
        glm::vec4 m_Color;
    public:
        GraphicsComponent(Entity* entityHandle);

        GraphicsComponent(Entity* entityHandle, const glm::vec4& color);

        const glm::vec4& GetColor();

        void OnUpdate() override;
    };
}

//class 
#endif // COMPONENTS_H
