#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "HartEng/ECS/Component.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace HE
{
    class TransformComponent: public Component
    {
    private:
        glm::mat4 m_Transform = glm::mat4(1.0f);
        glm::vec3 m_Translation = glm::vec3(0.0f);
        glm::quat m_Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 m_Scale = glm::vec3(1.0f);

        void Recalculate();
    public:
        TransformComponent(Entity* entityHandle);
        TransformComponent(Entity* entityHandle, const glm::mat4& transform);
        virtual ~TransformComponent() = default;

        void SetTranslation(const glm::vec3& translate);
        void SetRotation(const glm::quat& rotation);
        void SetRotation(const glm::vec3& angles);
        void SetScale(const glm::vec3& scale);
        void SetTransform(const glm::mat4& transform);
        void SetTRC(const glm::vec3& translate, const glm::vec3& angles, const glm::vec3& scale);

        const glm::vec3& GetTranslation() const;
        const glm::quat& GetRotation() const;
        const glm::vec3& GetScale() const;
        const glm::mat4& GetTransform() const;

    };
}

#endif // TRANSFORM_H
