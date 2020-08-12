#ifndef CAMERAS_H
#define CAMERAS_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace HE
{
    class OrthographicCamera
    {
    private:
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionViewMatrix;

        glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
        glm::quat m_Rotation = {0.0f, 0.0f, 0.0f, 0.0f};

        void RecalculateViewMatrix();
    public:
        OrthographicCamera(float left, float right, float bottom, float top);
        OrthographicCamera(float left, float right, float bottom, float top, float near, float far);

        void SetPosition(const glm::vec3& position);
        void SetRotation(float angle, const glm::vec3& rotation);

        const glm::vec3& GetPosition() const;
        const glm::quat& GetRotation() const;
        const glm::mat4& GetProjectionMatrix() const;
        const glm::mat4& GetViewMatrix() const;
        const glm::mat4& GetProjectionViewMatrix() const;
    };
}

#endif // CAMERAS_H
