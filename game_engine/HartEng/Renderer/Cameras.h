#ifndef CAMERAS_H
#define CAMERAS_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace HE
{

    class Camera
    {
    protected:
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionViewMatrix;

        glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
        glm::quat m_Rotation = {0.0f, 0.0f, 0.0f, 0.0f};

        void RecalculateViewMatrix();

    public:
        virtual ~Camera() = default;

        virtual void SetPosition(const glm::vec3& position);
        virtual void SetRotation(float angle, const glm::vec3& rotation);

        virtual const glm::vec3& GetPosition() const;
        virtual const glm::quat& GetRotation() const;
        virtual const glm::mat4& GetProjectionMatrix() const;
        virtual const glm::mat4& GetViewMatrix() const;
        virtual const glm::mat4& GetProjectionViewMatrix() const;
    };

    class OrthographicCamera: public Camera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top, float near = 0.0f, float far = 1.0f);

    };

    class PerspectiveCamera: public Camera
    {
    public:
        PerspectiveCamera(float fov, float aspect_ratio, float near, float far);
        PerspectiveCamera(float fov, float width, float heigth, float near, float far);

    };
}

#endif // CAMERAS_H
