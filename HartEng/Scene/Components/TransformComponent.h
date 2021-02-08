#pragma once

#include "HartEng/Scene/Component.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace HE
{
    class TransformComponent: public Component
    {
    private:
        glm::mat4 m_Transform{ 1.0f };
        glm::vec3 m_Position{ 0.0f };
        glm::vec3 m_Rotation{ 0.0f };
        //glm::quat m_Rotation{1.0f, 0.0f, 0.0f, 0.0f};
        glm::vec3 m_Scale{ 1.0f };

        void Recalculate();
    public:
        TransformComponent(Entity* entityHandle);
        virtual ~TransformComponent() = default;

        void SetPosition(const glm::vec3& position);
        //void SetRotation(const glm::quat& position);
        void SetRotation(const glm::vec3& angles);
        void SetScale(const glm::vec3& scale);
        void SetTransform(const glm::mat4& transform);
        void SetPRS(const glm::vec3& positin, const glm::vec3& angles, const glm::vec3& scale);

        const glm::vec3& GetPosition() const;
        const glm::vec3& GetRotation() const;
        //const glm::quat& GetRotationQ() const;
        const glm::vec3& GetScale() const;
        const glm::mat4& GetTransform() const;

    };
}

